using CL_LOG;
using FRRJIf;
using SFR_Robot_Planner_Lib.Model;
using SFR_Robot_Planner_Lib.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SFR_Robot_Planner_Online_Lib.Model
{
    public class RobotDef
    {
        private string ipAddr;
        private int _sleepTime;

        FRRJIf.Core mobjCore;
        FRRJIf.DataTable mobjDataTable;
        FRRJIf.DataCurPos mobjCurPos;
        List<FRRJIf.DataNumReg> mobjReg;

        //寄存器对应地址
        public int robotDisconnectCount = 0;
        public int alarmCount = 10;
        Array xyzwpr = new float[9];
        Array config = new short[7];
        public Array joint = new float[9];
        short intUF = 0;
        short intUT = 0;
        short intValidC = 0;
        short intValidJ = 0;
        FRRJIf.DataSysVarPos mobjDataSYSVAR_POS_JOGFRAMES_USER;

        //设备相关信息
        public int RobotDisconnectMaxCount;
        public int RobotConnectPeriodMax;
        public int RobotConnectPeriod;
        public bool status = false; //true:connect false:fail to connect

        //采集数据
        public bool IsDisconnected = false; //多次连接失败后，判断为断开连接

        public string CurrentStatus;

        private int m_userFrame;

        public RobotDef(string ipAddress, int cycleTime, int userFrame)
        {
            ipAddr = ipAddress;
            _sleepTime = cycleTime;

            m_userFrame = userFrame;
            this.status = false;
            this.CurrentStatus = RobotStatus.PowerOff;
            this.mobjReg = new List<DataNumReg>();

            initial();
        }

        public void Dispose()
        {
            this.mobjCore = null;
            this.mobjDataTable = null;
        }

        public bool Connect()
        {
            bool res = false;

            try
            {
                RobotDisconnect();

                //先尝试IP是否能PING通
                bool ret_ping = WebUtil.tryPing(ipAddr);
                if (ret_ping)
                {
                    initial();
                    Thread.Sleep(100);
                    this.status = RobotConnect(ipAddr);
                    if (this.status)
                    {
                        RLogger.Instance.LogINFOString($"Robot connect pass:{ipAddr}");

                        this.robotDisconnectCount = 0;
                        this.IsDisconnected = false;
                        return true;
                    }
                }
            }
            catch (Exception ex)
            {
                RLogger.Instance.LogERRORInformation($"RobotConnect error: {ex.Message}");
            }

            return res;
        }

        public bool RobotRead()
        {
            object tmpNumRegVal = 0;

            try
            {
                if (!this.status)
                {
                    RLogger.Instance.LogERRORInformation($"RobotRead:({ipAddr}) Status false");
                    return false;
                }
                else
                {
                    //Refresh data table
                    if (!mobjDataTable.Refresh())
                    {
                        RLogger.Instance.LogERRORInformation($"RobotRead:({this.ipAddr}) Refresh Failed");
                        this.status = false;
                        this.CurrentStatus = RobotStatus.PowerOff;
                        return false;
                    }

                    //current position
                    if (!this.mobjCurPos.GetValue(ref xyzwpr, ref config, ref joint, ref intUF, ref intUT, ref intValidC, ref intValidJ))
                    {
                        RLogger.Instance.LogERRORInformation($"RobotRead:({this.ipAddr}) CurrentPos Failed");
                        this.status = false;
                        this.CurrentStatus = RobotStatus.PowerOff;
                        return false;
                    }


                }
                return true;
            }
            catch (Exception ex)
            {
                RLogger.Instance.LogERRORInformation($"RobotRead:({this.ipAddr}) {ex.Message}");
                return false;
            }
        }

        public void setUserFrame(float xCoord, float yCoord, float rCoord) 
        {
            if (mobjDataTable.Refresh()) 
            {
                mobjDataSYSVAR_POS_JOGFRAMES_USER.GetValueXyzwpr(xyzwpr, config, ref intUF, ref intUT, ref intValidJ);
                xyzwpr.SetValue(xCoord ,0);
                xyzwpr.SetValue(yCoord, 1);
                xyzwpr.SetValue(rCoord, 5);
                mobjDataSYSVAR_POS_JOGFRAMES_USER.SetValueXyzwpr(ref xyzwpr, ref config, intUF, intUT);
            }
        }

        private bool RobotConnect(string strHost)
        {
            bool ret = false;
            try
            {
                ret = this.mobjCore.Connect(strHost);
                RLogger.Instance.LogINFOString($"Robot Connect Result: {ret}");
            }
            catch (Exception ex)
            {
                RLogger.Instance.LogERRORInformation($"RobotConnect Exception {ex.Message}");
            }
            return ret;
        }

        public bool RobotDisconnect()
        {

            bool ret = false;
            try
            {
                ret = this.mobjCore.Disconnect();
            }
            catch (Exception ex)
            {
                RLogger.Instance.LogERRORInformation($"RobotDisconnect Exception {ex.Message}");
            }
            this.IsDisconnected = true;
            return ret;
        }

        public bool initial()
        {
            try
            {
                this.mobjCore = null;
                this.mobjDataTable = null;
                this.mobjCurPos = null;
                this.mobjDataSYSVAR_POS_JOGFRAMES_USER = null;
                this.mobjReg = null;
                this.mobjCore = new Core();
                this.mobjCore.set_TimeOutValue(1000);
                this.mobjDataTable = this.mobjCore.get_DataTable();
                this.mobjCurPos = this.mobjDataTable.AddCurPosUF(FRRJIf.FRIF_DATA_TYPE.CURPOS, 1, 0);
                this.mobjDataSYSVAR_POS_JOGFRAMES_USER = mobjDataTable.AddSysVarPos(FRIF_DATA_TYPE.SYSVAR_POS, $"$MNUFRAME[1,{m_userFrame}]");

                RLogger.Instance.LogINFOString($"Robot initial pass:{ipAddr}");
                return true;
            }
            catch (Exception ex)
            {
                RLogger.Instance.LogERRORInformation($"Robot initial fail:{ex.Message.ToString()}");
                return false;
            }
        }

        public static bool tryPingRobotDef(string ipAddr)
        {
            bool returnFlag = false;
            using (PingDef ping = new PingDef())
            {
                if (ping.pingResult(ipAddr) == PingDef.replyEnum.success)
                {
                    returnFlag = true;
                }
                else
                {
                    returnFlag = false;
                }
            }
            return returnFlag;
        }

        public class alarmObj
        {
            public string alarmType;
            public string alarmCode;
            public string alarmMessage;
            public string alarmSeverityMessage;
            public int alarmSeverityNum;
            public string alarmCauseMessage;
            public DateTime time;

            public alarmObj()
            {
                this.alarmInitial();
            }

            public void alarmInitial()
            {
                this.alarmType = "";
                this.alarmCode = "";
                this.alarmMessage = "";
                this.alarmSeverityMessage = "";
                this.alarmSeverityNum = 0;
                this.alarmCauseMessage = "";
                this.time = DateTime.Now;
            }
        }

        private static class RobotStatus
        {
            public const string Run = "Run";
            public const string Fault = "Fault";
            public const string LoadWait = "LoadWait";
            public const string PowerOff = "PowerOff";
        }
    }
}
