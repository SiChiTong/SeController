/*
 * ControllerApplication.h
 *
 *  Created on: 2016年10月15日
 *      Author: seeing
 */

#ifndef _CONTROLLERAPPLICATION_H_
#define _CONTROLLERAPPLICATION_H_

#include <Application/Application.h>
#include <DataSet/DataType/DataBase.h>
#include <Service/ServiceType/ServiceOdometry.h>
#include <Service/ServiceType/ServiceTransform.h>
#include <Transform/LinearMath/Transform.h>
#include <DataSet/DataType/Odometry.h>
#include <DataSet/DataType/Twist.h>
#include <Time/Time.h>
#include <Time/Duration.h>
#include <Service/Server.h>
#include <DataSet/Subscriber.h>

#include <boost/thread/thread.hpp>

#include "Communication/SpiComm.h"

#include "ExtendedKalmanFilter/Estimation.h"

namespace NS_Controller
{

  typedef struct
  {
    double x;
    double y;
    double theta;
    double linear_vel;
    double angular_vel;
    double roll;
    double pitch;
    double yaw;
  } PoseState;

  class ControllerApplication: public Application
  {
  public:
    ControllerApplication();
    virtual
    ~ControllerApplication();
  private:
    SpiComm* comm;

    boost::mutex base_lock;

    boost::thread get_pose_thread;

    PoseState original_pose;

    NS_DataType::Odometry current_odometry;
    NS_Transform::Transform current_odom_transform;

    OdomEstimation estimation;

    NS_Service::Server< NS_ServiceType::ServiceOdometry >* odom_srv;
    NS_Service::Server< NS_ServiceType::ServiceTransform >* odom_tf_srv;

    NS_DataSet::Subscriber< NS_DataType::Twist >* twist_sub;
  private:
    std::string comm_dev_name_;

    double wheel_diameter_;
    int encoder_resolution_;
    int gear_reduction_;
    double wheel_track_;
    double accel_limit_;

    int control_timeout_;

    /*
     * PID parameters
     */
    double pid_kp_right_;
    double pid_kd_right_;
    double pid_ki_right_;
    double pid_ko_right_;
    double pid_max_right_;
    double pid_min_right_;

    double pid_kp_left_;
    double pid_kd_left_;
    double pid_ki_left_;
    double pid_ko_left_;
    double pid_max_left_;
    double pid_min_left_;

    int control_duration_;

    bool use_ekf_;

  private:

    void
    odomService(NS_ServiceType::ServiceOdometry& odometry);
    void
    odomTransformService(NS_ServiceType::ServiceTransform& transform);

    void
    velocityCallback(NS_DataType::Twist& twist);

    void
    loadParameters();
    void
    configController();

    bool
    checkDevice();

    PoseState
    getBasePose();

    void
    getPoseLoop(double frequency);

    void
    estimate();

  public:
    virtual void
    run();
    virtual void
    quit();
  };

} /* namespace NS_Controller */

#endif /* CONTROLLER_COTROLLERAPPLICATION_H_ */
