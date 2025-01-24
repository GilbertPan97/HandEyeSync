#include "PsAlgorithm.h"
#include "utils.h"

#include <iostream>
#include <cmath>

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <unsupported/Eigen/KroneckerProduct>

namespace ProfileScanner
{
    algorithm::algorithm(Constructor type, SolveMethod method_){
        construct_type_ = type;
        solve_method_ = method_;
    }

    bool algorithm::Simultaneous_Calib(std::vector<Eigen::Matrix4f> htm_end2base, 
                                        std::vector<Eigen::Vector3f> p_cam, 
                                        Eigen::Matrix3f &R_solution,
                                        Eigen::Vector3f &t_solution){
        // get calib dataset info
        if (htm_end2base.size() == p_cam.size()){
            nbr_data_ = htm_end2base.size();
            mtr_end2base_ = htm_end2base, ctr_pnts_ = p_cam;
        } else {
            std::cout << "ERROR: calib dataset is not match. \n";
            return false;
        }

        // calculate execution
        if (solve_method_ == SolveMethod::ITERATION){
            // 1. set iterative initial value
            Eigen::Matrix3f Rx_init = Eigen::Matrix3f::Identity();
            Eigen::Vector3f tx_init(1.0, 1.0, 1.0);

            // 2. iterative calibrate
            float error;
            int iter = 0;
            do{
                // construct linear equation: F * delta = q
                // calculate delta: delta_R & delta_t
                // delta: deviation between initial iterative and true value
                // n data can only build n-1 equation, which can be stack to F & q
                Eigen::MatrixXf F(3 * (nbr_data_ - 1), 6);
                Eigen::VectorXf q(3 * (nbr_data_ - 1));
                construct_linear_equation_iter(Rx_init, tx_init, F, q);

                // linear fit to solve equation
                Eigen::MatrixXf F_pinv = F.completeOrthogonalDecomposition().pseudoInverse();
                Eigen::VectorXf delta;
                delta = F_pinv * q;

                // iterative initial value
                Eigen::Vector3f delta_R = delta.block<3, 1>(0, 0);
                Eigen::Vector3f delta_t = delta.block<3, 1>(3, 0);
                Rx_init = CalibUtils::rodrigues(delta_R) * Rx_init;
                tx_init = tx_init + delta_t;

                error = delta.norm();
                iter++;
            }while(error >= 1.0e-4 && iter <= 300);

            // 3. output solve result
            R_solution = Rx_init, t_solution = tx_init;
            std::cout << "INFO: Iterative solution is: \n" << R_solution << std::endl
                        << "translation component: " << t_solution.transpose() << std::endl;
        } 

        else if(solve_method_ == SolveMethod::REGRESSION){
            Eigen::MatrixXf F(3 * (nbr_data_ - 1), 12);
            Eigen::VectorXf q(3 * (nbr_data_ - 1));
            construct_linear_equation_kron(F, q);

            // linear fit to solve equation
            Eigen::MatrixXf F_pinv = F.completeOrthogonalDecomposition().pseudoInverse();
            auto solutions = F_pinv * q;        // solutions = [vec(Rx); tx], size: 12, 1
            std::cout << "INFO: Regression solution is: \n" << solutions.transpose() << std::endl;

            // calculate regression error
            Eigen::VectorXf vec_err = q - F * solutions;
            float error = vec_err.norm()/nbr_data_;
            std::cout << "INFO: Regression error is: \n" << vec_err.transpose() << std::endl;

            // restore Rx and tx
            R_solution << solutions.block<3,1>(0, 0),
                            solutions.block<3,1>(3, 0),
                            solutions.block<3,1>(6, 0);
            R_solution = svd_orth(R_solution);
            t_solution << solutions.block<3,1>(9, 0);

            std::cout << "INFO: R_ort solution is: \n" << R_solution << std::endl; 
        } 
        
        else{
            std::cout << "ERROR: Wrong solve method.\n";
            exit(-1);
        }

        return true;
    }

    bool algorithm::Separate_Calib(std::vector<Eigen::Matrix4f> htm_end2base0,
                                    std::vector<Eigen::Vector3f> p_cam0, 
                                    std::vector<Eigen::Matrix4f> htm_end2base1,
                                    std::vector<Eigen::Vector3f> p_cam1,
                                    Eigen::Matrix3f & R_solution,
                                    Eigen::Vector3f & t_solution){

        if(solve_method_ == SolveMethod::REGRESSION){

            /* ======= 1. Recovering the rotation ======= */ 
            size_t nbr_calibR = htm_end2base0.size();
            std::vector<Eigen::Matrix3f> Rb_stack(nbr_calibR);      // Rb: tool frame rotation in robot base (constant)
            std::vector<Eigen::Vector3f> pb_stack(nbr_calibR);      // pb: tool frame position in robot base (variable)
            std::vector<Eigen::Vector3f> vt_stack(nbr_calibR - 1);  // block edge vector (tool frame: robot flange)
            std::vector<Eigen::Vector3f> vs_stack(nbr_calibR - 1);  // vlock edge vector (sensor frame: camera)
            for (size_t i = 0; i < nbr_calibR; i++){
                CalibUtils::HomogeneousMtr2RT(htm_end2base0[i], Rb_stack[i], pb_stack[i]);
                
                // calculate vector from first point to ps_stack[i] (i > 0)
                if (i != 0){
                    vt_stack[i - 1] = pb_stack[i] - pb_stack[0];
                    vs_stack[i - 1] = p_cam0[i] - p_cam0[0];
                } 
            }

            // TODO: Calculate Rb: Rb_stack average, Rb should be constant, which mean Rb_stack[i] are the same
            auto Rb =  Rb_stack[0];
            Eigen::MatrixXf Hr(3*(vt_stack.size()-1), 9);
            Eigen::VectorXf br(3*(vt_stack.size()-1));
            for (size_t i = 0; i < vt_stack.size()-1; i++){
                size_t j = i + 1;
                auto B1 = Eigen::kroneckerProduct(vs_stack[i].cross(vs_stack[j]).transpose(), Rb);
                auto B2 = Eigen::kroneckerProduct(vs_stack[i].transpose(), Rb);
                auto B3 = Eigen::kroneckerProduct(vs_stack[j].transpose(), Rb);
                auto bri = -vt_stack[i].cross(vt_stack[j]);

                auto A1 = B1;
                auto A2 = CalibUtils::skew(vt_stack[j]).transpose() * B2;
                auto A3 = CalibUtils::skew(vt_stack[i]).transpose() * B3;
                
                auto Hri = A1 + A2 - A3;

                Hr.block<3, 9>(3 * i, 0) = Hri;
                br.block<3, 1>(3 * i, 0) = bri;
            }
            // std::cout << "INFO: H is:\n" << Hr << std::endl;
            // Eigen::FullPivLU<Eigen::MatrixXf> lu(Hr);
            // std::cout << "INFO: The rank of H is: " << lu.rank() << std::endl;

            Eigen::MatrixXf Hr_pinv = Hr.completeOrthogonalDecomposition().pseudoInverse();
            auto R_vec = Hr_pinv * br;        // R_vec = vec(Rx) size: 9, 1
            R_solution << R_vec.block<3,1>(0, 0),
                          R_vec.block<3,1>(3, 0),
                          R_vec.block<3,1>(6, 0);
            auto R_schmidt = rota_schmidt_orth(R_solution);
            auto R_svd = svd_orth(R_solution);
            std::cout << "======================== Orthogonal Rotation Matrix ========================\n";
            std::cout << "INFO: Original rotation component is:\n [" << R_solution << " ].\n";
            std::cout << "INFO: Rotation_schmidt is:\n [" << R_schmidt << " ].\n"
                      << "INFO: Rotation_svd is:\n [" << R_svd << " ].\n";
            std::cout << "============================================================================\n";
            std::cout << "INFO: Rotation calibration errors:\n" << (br - Hr * R_vec).transpose() << std::endl;

            /* ======= 2. Recovering block edge vector ======= */ 
            std::vector<Eigen::Vector3f> vb_stack;
            Eigen::Vector3f vb = {0, 0, 0};
            std::cout << "INFO: edge vector in robot base frame:\n";
            for (size_t i = 0; i < vt_stack.size(); i++){
                Eigen::Vector3f vb_i = (Rb * R_solution * vs_stack[i] + vt_stack[i]).normalized();
                vb = vb + vb_i;
                std::cout << "INFO: vb-[" << i << "] is: "
                          << vb_i.transpose() << std::endl;
                vb_stack.push_back(vb_i);
            }
            // vb = vb / vb_stack.size();
            vb = vb_stack[0];

            /* ======= 3. Recovering the translation ======= */
            size_t nbr_calibT = htm_end2base1.size();
            std::vector<Eigen::Matrix3f> Rb_stack1(nbr_calibT);      // Rb: tool frame rotation in robot base (constant)
            std::vector<Eigen::Vector3f> pb_stack1(nbr_calibT);      // pb: tool frame position in robot base (variable)
            for (size_t i = 0; i < nbr_calibT; i++){
                CalibUtils::HomogeneousMtr2RT(htm_end2base1[i], Rb_stack1[i], pb_stack1[i]);
            }
            std::vector<Eigen::Vector3f> ps_stack1 = p_cam1;         // ps: feature point coordinate in sensor (camera) frame

            Eigen::MatrixXf Ht(3 * (Rb_stack1.size() - 1), 3);
            Eigen::VectorXf bt(3 * (Rb_stack1.size() - 1));
            for (size_t i = 0; i < Rb_stack1.size()-1; i++){
                size_t j = i + 1;
                auto Hti = CalibUtils::skew(vb).transpose() * (Rb_stack1[j] - Rb_stack1[i]);
                auto bti = -(Rb_stack1[j] * R_solution * ps_stack1[j] - Rb_stack1[i] * R_solution *
                                ps_stack1[i] + pb_stack1[j] - pb_stack1[i]).cross(vb);

                Ht.block<3, 3>(3 * i, 0) = Hti;
                bt.block<3, 1>(3 * i, 0) = bti;
            }
            Eigen::MatrixXf Ht_pinv = Ht.completeOrthogonalDecomposition().pseudoInverse();
            t_solution = Ht_pinv * bt;
        }

        return true;
    }

    void algorithm::construct_linear_equation_iter(Eigen::Matrix3f Rx_init, Eigen::Vector3f tx_init,
                                                    Eigen::MatrixXf & F, Eigen::VectorXf & q){
        // check F & q size
        if(F.rows() != 3 * (nbr_data_ - 1) || F.cols() != 6){
            F.resize(3 * (nbr_data_ - 1), 6);
            std::cout << "WARNING: The size of 'F' is illegal, which is adjusted. \n";
        };
        if(q.size() != 3 * (nbr_data_ - 1)){
            q.resize(3 * (nbr_data_ - 1));
            std::cout << "WARNING: The size of 'q' is illegal, which is adjusted. \n";
        };

        // construct equation
        Eigen::Matrix3f Ra_i, Ra_j;     // j = i-1
        Eigen::Vector3f ta_i, ta_j, tsc_i, tsc_j;
        for (size_t i = 1; i < nbr_data_; i++)
        {
            // allocate data
            CalibUtils::HomogeneousMtr2RT(mtr_end2base_[i], Ra_i, ta_i);
            CalibUtils::HomogeneousMtr2RT(mtr_end2base_[i-1], Ra_j, ta_j);
            tsc_i = ctr_pnts_[i];
            tsc_j = ctr_pnts_[i-1];

            // calculate linear equation coefficient (Fi)
            Eigen::Matrix3f F_Ri;    // coefficient of delta_R
            Eigen::Matrix3f F_ti;    // coefficient of delta_t
            F_Ri = -Ra_i * CalibUtils::skew(Rx_init * tsc_i)+ 
                    Ra_j * CalibUtils::skew(Rx_init * tsc_j);
            F_ti = Ra_i - Ra_j;

            Eigen::MatrixXf Fi(3, 6);
            Fi.block<3, 3>(0, 0) = F_Ri;
            Fi.block<3, 3>(0, 3) = F_ti;

            // calculate linear value (qi) of equation 
            Eigen::Vector3f qi;
            qi = -(Ra_i * Rx_init * tsc_i + Ra_i * tx_init + ta_i) +
                    (Ra_j * Rx_init * tsc_j + Ra_j * tx_init + ta_j);

            // stack Fi and qi back to F and q (following to row)
            F.block<3, 6>(3 * (i-1), 0) = Fi;
            q.block<3, 1>(3 * (i-1), 0) = qi;
        }
        // std::cout << "F block is: \n" << F << std::endl;
        // std::cout << "q block is: \n" << q << std::endl;
    }

    void algorithm::construct_linear_equation_kron(Eigen::MatrixXf & F, Eigen::VectorXf & q){
        // check F & q size
        if(F.rows() != 3 * (nbr_data_ - 1) || F.cols() != 12){
            F.resize(3 * (nbr_data_ - 1), 12);
            std::cout << "WARNING: The size of 'F' is illegal, which is adjusted. \n";
        };
        if(q.size() != 3 * (nbr_data_ - 1)){
            q.resize(3 * (nbr_data_ - 1));
            std::cout << "WARNING: The size of 'q' is illegal, which is adjusted. \n";
        };

        // construct equation
        Eigen::Matrix3f Ra_i, Ra_j;     // j = i-1
        Eigen::Vector3f ta_i, ta_j, tsc_i, tsc_j;
        for (size_t i = 1; i < nbr_data_; i++)
        {
            // allocate data
            CalibUtils::HomogeneousMtr2RT(mtr_end2base_[i], Ra_i, ta_i);
            CalibUtils::HomogeneousMtr2RT(mtr_end2base_[i-1], Ra_j, ta_j);
            tsc_i = ctr_pnts_[i];
            tsc_j = ctr_pnts_[i-1];

            // calculate linear equation coefficient (Fi)
            auto kro_i = Eigen::kroneckerProduct(tsc_i.transpose(), Ra_i);
            auto kro_j = Eigen::kroneckerProduct(tsc_j.transpose(), Ra_j);

            Eigen::MatrixXf Fi(3, 12);
            Fi.block<3, 9>(0, 0) << kro_j - kro_i;
            Fi.block<3, 3>(0, 9) << Ra_j - Ra_i;

            // std::cout << "Kro_j - Kro_i is: \n" << kro_j - kro_i << std::endl;
            // std::cout << "Ra_j - Ra_i is: \n" << Ra_j - Ra_i << std::endl;
            // std::cout << "F_i is: \n" << Fi << std::endl;

            // calculate linear value (qi) of equation 
            Eigen::Vector3f qi = -(ta_j - ta_i);

            // stack Fi and qi back to F and q (following to row)
            F.block<3, 12>(3 * (i-1), 0) = Fi;
            q.block<3, 1>(3 * (i-1), 0) = qi;
        }
        // std::cout << "F block is: \n" << F << std::endl;
        // std::cout << "q block is: \n" << q << std::endl;
    }

    Eigen::Matrix3f algorithm::rota_schmidt_orth(Eigen::Matrix3f r_mat){
        Eigen::Vector3f alpha1 = r_mat.col(0);
        Eigen::Vector3f alpha2 = r_mat.col(1);
        Eigen::Vector3f alpha3 = r_mat.col(2);

        Eigen::Vector3f beta1 = alpha1;
        Eigen::Vector3f beta2 = alpha2 - alpha2.dot(beta1)/beta1.dot(beta1) * beta1;
        Eigen::Vector3f beta3 = alpha3 - alpha3.dot(beta1)/beta1.dot(beta1) * beta1 - 
                                alpha3.dot(beta2)/beta2.dot(beta2) * beta2;
        
        beta1.normalize(), beta2.normalize(), beta3.normalize();

        // TODO: need to delete -
        Eigen::Matrix3f r_orth_mat;
        r_orth_mat << beta1, beta2, beta3;

        if (!CalibUtils::isRotatedMatrix(r_orth_mat)){
            std::cout << "ERROR: Schmidt orthogonalization return false result. "
                      << "R_orth_mat determinant is: " << r_orth_mat.determinant();
            exit(-1);
        }
        
        return r_orth_mat;
    }

    Eigen::Matrix3f algorithm::svd_orth(Eigen::Matrix3f r_mat){

        Eigen::FullPivLU<Eigen::Matrix3f> lu(r_mat);
        if(lu.rank() < 3){
            std::cout << "ERROR: Rotation matrix rank is less than 3.\n";
            exit(-1);
        }

        Eigen::JacobiSVD<Eigen::Matrix3f> svd(r_mat, Eigen::ComputeFullU | Eigen::ComputeFullV);
        Eigen::Matrix3f sigValueRectify = Eigen::Matrix3f::Identity();
        Eigen::Matrix3f mtr_U = svd.matrixU();
        Eigen::Matrix3f mtr_V = svd.matrixV();
        sigValueRectify(2, 2) = mtr_U.determinant() * mtr_V.determinant();

        Eigen::Matrix3f r_mat_orth;
        r_mat_orth = mtr_U * sigValueRectify * mtr_V.transpose();

        // rectify Rotation matrix to satisfy: r_mat_orth.determinant() = 1
        float detSign = r_mat_orth.determinant() > 0 ? 1 : -1;
        r_mat_orth *= detSign;
        
        if (!CalibUtils::isRotatedMatrix(r_mat_orth)){
            std::cout << "ERROR: Schmidt orthogonalization return false result. "
                      << "R_orth_mat determinant is: " << r_mat_orth.determinant();
            exit(-1);
        }

        return r_mat_orth;
    }

} // namespace ProfileScanner