#ifndef INC_CONFIG_HPP
#define INC_CONFIG_HPP

#define FREQ_NORMALSPIN 800
#define RAIL_DISTANCE 635.0           // mm
#define DELRIN_SPACER_DISTANCE 64.0   // mm
#define WALK_TO_SCENE_DISTANCE 1000   // Steps to walk to the scene
#define LEAVE_THE_SCENE_DISTANCE 1000 // Steps to leave the scene

/* For FreeRTOS purpose */
#define USART_COMM_TASK_HEAP 1024
#define PLAY_TASK_HEAP 1024
#define VSLOT_CALIBRATION_TASK_HEAP 1024
#define WAYANGSERVO_TASK_HEAP 1024

#endif // INC_CONFIG_HPP