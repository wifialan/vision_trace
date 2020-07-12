#ifndef STM32RANGE_H
#define STM32RANGE_H
#include "RangeData.h"
#include "rs232.h"
#include <math.h>
#include <vector>
#define RECV_BUF_SIZE 16
typedef enum SensorNo {
    FRONT_LEFT = 0,
    FRONT_MID_1 = 1,
    FRONT_MID_2 = 2,
    FRONT_RIGHT = 3
}emSensorNo;
typedef enum RangeSensorType{
    ULTRASONIC = 0,
    INFRARED = 1
}emRangeSensorType;
class STM32Range{
public:
    STM32Range();
    ~STM32Range();
    void CreateRs232Object();
    bool OpenRs232();
    bool OpenSuccess_;
    std::vector<RangeData> rangeData_vec_;
    bool RecvRangeData();
    void add_range_data(SensorNo sensorNo,unsigned short len);
private:
    RS232 *pRs232FrontBoard_;
    unsigned char front_recv_[RECV_BUF_SIZE];
    void CloseRs232();
    bool verifyData();
    bool verifyData(unsigned char* data);

};

#endif // STM32RANGE_H
