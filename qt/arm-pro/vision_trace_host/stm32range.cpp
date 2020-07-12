#include "stm32range.h"
STM32Range::STM32Range(){
    rangeData_vec_.clear();
    OpenSuccess_ = false;
    memset(&front_recv_,0,sizeof(unsigned char) * RECV_BUF_SIZE);
    CreateRs232Object();
}
STM32Range::~STM32Range(){
    CloseRs232();
}
void STM32Range::CreateRs232Object(){
    pRs232FrontBoard_ = new RS232();
    memset(&front_recv_,0,sizeof(unsigned char) * RECV_BUF_SIZE);
}
bool STM32Range::OpenRs232(){

    if(pRs232FrontBoard_->OpenDevice("/dev/rangeFront",9600)) {
        return true;
    }
    else {
        return false;
    }

}
void STM32Range::CloseRs232(){
    pRs232FrontBoard_->CloseDevice();
    delete pRs232FrontBoard_;
}
bool STM32Range::RecvRangeData(){
    int recvLen = 0;

    recvLen = pRs232FrontBoard_->Read(front_recv_,12);
    if(verifyData(front_recv_)){
        unsigned short left = (front_recv_[2] << 8 ) | front_recv_[3];
        unsigned short mid_1 = (front_recv_[4] << 8 ) | front_recv_[5];
        unsigned short mid_2 = (front_recv_[6] << 8 ) | front_recv_[7];
        unsigned short right = (front_recv_[8] << 8 ) | front_recv_[9];
        add_range_data(FRONT_LEFT,left);
        add_range_data(FRONT_MID_1,mid_1);
        add_range_data(FRONT_MID_2,mid_2);
        add_range_data(FRONT_RIGHT,right);

    }else{
        //LOG(ERROR) << "front board recv msg failed!";
    }

    return false;
}
bool STM32Range::verifyData(unsigned char* data){
     return true;
}
void STM32Range::add_range_data(SensorNo sensorNo,unsigned short len){
    RangeData rangeData;
    rangeData.stamp = 0.0;
    rangeData.radiation_type = (int)INFRARED;
    rangeData.field_of_view = 2 * M_PI / 180;
    rangeData.max_range = 2.0;
    rangeData.min_range = 0.0;
    rangeData.rangeNo = (int)sensorNo;
    rangeData.range = len / 1000.0;
    rangeData_vec_.push_back(rangeData);
}
