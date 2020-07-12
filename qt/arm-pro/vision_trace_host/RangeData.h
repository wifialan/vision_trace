/**********************************************************************
 * 北京零维时空科技有限公司(iGoSens(BeiJing) CO.Ltd.).
 * Copyright © 2017 All rights reserved.
 * Created by Tony Fang(fangjianwei@igosens.com)
 *            Cathy Yang(yangcheng@igosens.com).
 **********************************************************************
 * 说明： MAP数据类型
 **********************************************************************/

#ifndef SENSORMANAGER_RANGEDATA_H
#define SENSORMANAGER_RANGEDATA_H

//#include <common/iGoType.h>
#include <memory>

struct RangeData{
    double stamp;
    unsigned char  radiation_type; // 0: ULTRASOUND 1: INFRARED
    float  field_of_view;
    float  min_range;
    float  max_range;
    float  range;
    int rangeNo;

    RangeData()
            :stamp(0.0),
             radiation_type(0),
             field_of_view(0.0),
             min_range(0.0),
             max_range(0.0),
             range(0.0),
             rangeNo(0){
    }
};

typedef std::shared_ptr<RangeData> RangeDataPtr;
typedef std::shared_ptr<RangeData const> RangeDataConstPtr;


#endif //SENSORMANAGER_RANGEDATA_H
