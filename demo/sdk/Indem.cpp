#include <mutex>
#include <queue>
#include <thread>
#include <iomanip>
#include <string>
#include <iostream>
#include <cmath>
#include "imrsdk.h"

using namespace indem;

struct ImrDepthImageTarget
{
	float _cubesize;
	int _image_w;
	int _image_h;
	float* _deepptr;
};

void PrintModuleInfo(CIMRSDK* pSDK)
{
    ImrModuleDeviceInfo info = pSDK->GetModuleInfo();
    std::cout << "Module Detail Info: \n"
        << "  ID: " << info._id << std::endl
        << "  Designed By: " << info._designer << std::endl
        << "  BaseLine: " << info._baseline << std::endl
        << "  Firmware Version: " << info._firmware_version << std::endl
        << "  Hardware Version: " << info._hardware_version << std::endl
        << "  IMU: " << info._imu << std::endl
        << "  Lens: " << info._lens << std::endl
        << "  View Angle: " << info._viewing_angle << std::endl;
}

void PrintEach(int row, int col, double* ptr)
{
    for (int r = 0; r < row; ++r)
    {
        for (int c=0;c<col;++c)
        {
            std::cout << ptr[r*col+c]<<"\t";
        }
        std::cout << std::endl;
    }
}

void PrintModuleParameters(CIMRSDK* pSDK)
{
    CameraCalibrationParameter params = pSDK->GetModuleParams();
    std::cout << "ACC: " << std::endl;
    PrintEach(3, 4, params._Acc);
    std::cout << "Gyr: " << std::endl;
    PrintEach(3, 4, params._Gyr);
    std::cout << "Dl: " << std::endl;
    PrintEach(4, 1, params._Dl);
    std::cout << "Dr: " << std::endl;
    PrintEach(4, 1, params._Dr);
    std::cout << "Kl: " << std::endl;
    PrintEach(3, 3, params._Kl);
    std::cout << "Kr: " << std::endl;
    PrintEach(3, 3, params._Kr);
    std::cout << "Pl: " << std::endl;
    PrintEach(3, 4, params._Pl);
    std::cout << "Pr: " << std::endl;
    PrintEach(3, 4, params._Pr);
    std::cout << "Rl: " << std::endl;
    PrintEach(3, 3, params._Rl);
    std::cout << "Rr: " << std::endl;
    PrintEach(3, 3, params._Rr);
    std::cout << "TSCl: " << std::endl;
    PrintEach(4, 4, params._TSCl);
    std::cout << "TSCr: " << std::endl;
    PrintEach(4, 4, params._TSCr);
    std::cout << "Baseline: " << params._baseline << " m" << std::endl;
    std::cout << "AMax: " << params._AMax << std::endl;
    std::cout << "SigmaAC: " << params._SigmaAC << std::endl;
    std::cout << "SigmaBa: " << params._SigmaBa << std::endl;
    std::cout << "GMax: " << params._GMax << std::endl;
    std::cout << "SigmaAwC: " << params._SigmaAwC << std::endl;
    std::cout << "SigmaBg: " << params._SigmaBg << std::endl;
    std::cout << "SigmaGC: " << params._SigmaGC << std::endl;
    std::cout << "SigmaGwC: " << params._SigmaGwC << std::endl;
}

void DepthImageCallback(int ret, void* pData, void* pParam) {
	ImrDepthImageTarget* Depth = reinterpret_cast<ImrDepthImageTarget*>(pData);
        //std::cout << "DepthImageCallback==" << std::setprecision(10) << Depth->_image_w << ", _image_h" << Depth->_image_h << std::endl;
}

void  SdkCameraCallBack(double time, unsigned char* pLeft, unsigned char* pRight, int width, int height, int channel, void* pParam)
{
	//std::cout << "SdkCameraCallBack==" << std::setprecision(10) << time << std::endl;
}

void sdkImuCallBack(double time, float accX, float accY, float accZ, float gyrX, float gyrY, float gyrZ, void* pParam)
{
//	std::cout << "sdkImuCallBack==" << time << " " << accX << "  " << accY << "  " << accZ  << "  " << gyrX << "  " << gyrY << "  " << gyrZ  << std::endl;
}

void sdkSLAMResult(int ret, void* pData, void* pParam)
{
    ImrModulePose* pose = (ImrModulePose*)pData;
    std::cout << "SLAM: "<<pose->_pose._position[0] << " " << pose->_pose._position[1]  << " " << pose->_pose._position[2] << " "<< pose->_pose._oula[0] << " " << pose->_pose._oula[1]  << " " << pose->_pose._oula[2]  << std::endl;
}

int main()
{
//    using namespace indem;
    CIMRSDK* pSDK = new CIMRSDK();
    MRCONFIG config = { 0 };

    config.bSlam = true;   //true: open slam
    pSDK->Init(config);

    PrintModuleInfo(pSDK);
    PrintModuleParameters(pSDK);

    pSDK->RegistModuleCameraCallback(SdkCameraCallBack,NULL);
    pSDK->RegistModuleIMUCallback(sdkImuCallBack,NULL);
    pSDK->RegistModulePoseCallback(sdkSLAMResult,NULL);
//    pSDK->AddPluginCallback("depthimage", "depth", DepthImageCallback, NULL);


    std::this_thread::sleep_for(std::chrono::seconds(60 * 60 * 24));
    pSDK->Release();
    delete pSDK;
    std::cout << "-----------------------END--------------------" << std::endl;
     
    return 0;
}