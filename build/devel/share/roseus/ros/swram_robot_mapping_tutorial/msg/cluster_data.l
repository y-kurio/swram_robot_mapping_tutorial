;; Auto-generated. Do not edit!


(when (boundp 'swram_robot_mapping_tutorial::cluster_data)
  (if (not (find-package "SWRAM_ROBOT_MAPPING_TUTORIAL"))
    (make-package "SWRAM_ROBOT_MAPPING_TUTORIAL"))
  (shadow 'cluster_data (find-package "SWRAM_ROBOT_MAPPING_TUTORIAL")))
(unless (find-package "SWRAM_ROBOT_MAPPING_TUTORIAL::CLUSTER_DATA")
  (make-package "SWRAM_ROBOT_MAPPING_TUTORIAL::CLUSTER_DATA"))

(in-package "ROS")
;;//! \htmlinclude cluster_data.msg.html
(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass swram_robot_mapping_tutorial::cluster_data
  :super ros::object
  :slots (_cluster_number _header _cluster_type _cluster_points _orientation ))

(defmethod swram_robot_mapping_tutorial::cluster_data
  (:init
   (&key
    ((:cluster_number __cluster_number) (make-array 0 :initial-element 0 :element-type :integer))
    ((:header __header) (instance std_msgs::Header :init))
    ((:cluster_type __cluster_type) (make-array 0 :initial-element 0.0 :element-type :float))
    ((:cluster_points __cluster_points) (let (r) (dotimes (i 0) (push (instance geometry_msgs::PolygonStamped :init) r)) r))
    ((:orientation __orientation) (let (r) (dotimes (i 0) (push (instance geometry_msgs::Point :init) r)) r))
    )
   (send-super :init)
   (setq _cluster_number __cluster_number)
   (setq _header __header)
   (setq _cluster_type __cluster_type)
   (setq _cluster_points __cluster_points)
   (setq _orientation __orientation)
   self)
  (:cluster_number
   (&optional __cluster_number)
   (if __cluster_number (setq _cluster_number __cluster_number)) _cluster_number)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:cluster_type
   (&optional __cluster_type)
   (if __cluster_type (setq _cluster_type __cluster_type)) _cluster_type)
  (:cluster_points
   (&rest __cluster_points)
   (if (keywordp (car __cluster_points))
       (send* _cluster_points __cluster_points)
     (progn
       (if __cluster_points (setq _cluster_points (car __cluster_points)))
       _cluster_points)))
  (:orientation
   (&rest __orientation)
   (if (keywordp (car __orientation))
       (send* _orientation __orientation)
     (progn
       (if __orientation (setq _orientation (car __orientation)))
       _orientation)))
  (:serialization-length
   ()
   (+
    ;; int32[] _cluster_number
    (* 4    (length _cluster_number)) 4
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float32[] _cluster_type
    (* 4    (length _cluster_type)) 4
    ;; geometry_msgs/PolygonStamped[] _cluster_points
    (apply #'+ (send-all _cluster_points :serialization-length)) 4
    ;; geometry_msgs/Point[] _orientation
    (apply #'+ (send-all _orientation :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int32[] _cluster_number
     (write-long (length _cluster_number) s)
     (dotimes (i (length _cluster_number))
       (write-long (elt _cluster_number i) s)
       )
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float32[] _cluster_type
     (write-long (length _cluster_type) s)
     (dotimes (i (length _cluster_type))
       (sys::poke (elt _cluster_type i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;; geometry_msgs/PolygonStamped[] _cluster_points
     (write-long (length _cluster_points) s)
     (dolist (elem _cluster_points)
       (send elem :serialize s)
       )
     ;; geometry_msgs/Point[] _orientation
     (write-long (length _orientation) s)
     (dolist (elem _orientation)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int32[] _cluster_number
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cluster_number (instantiate integer-vector n))
     (dotimes (i n)
     (setf (elt _cluster_number i) (sys::peek buf ptr- :integer)) (incf ptr- 4)
     ))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float32[] _cluster_type
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cluster_type (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _cluster_type i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;; geometry_msgs/PolygonStamped[] _cluster_points
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _cluster_points (let (r) (dotimes (i n) (push (instance geometry_msgs::PolygonStamped :init) r)) r))
     (dolist (elem- _cluster_points)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;; geometry_msgs/Point[] _orientation
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _orientation (let (r) (dotimes (i n) (push (instance geometry_msgs::Point :init) r)) r))
     (dolist (elem- _orientation)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get swram_robot_mapping_tutorial::cluster_data :md5sum-) "19a74e2a774e33127a923df0183079be")
(setf (get swram_robot_mapping_tutorial::cluster_data :datatype-) "swram_robot_mapping_tutorial/cluster_data")
(setf (get swram_robot_mapping_tutorial::cluster_data :definition-)
      "int32[]  cluster_number
std_msgs/Header header
float32[] cluster_type
geometry_msgs/PolygonStamped[] cluster_points
geometry_msgs/Point[] orientation
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

================================================================================
MSG: geometry_msgs/PolygonStamped
# This represents a Polygon with reference coordinate frame and timestamp
Header header
Polygon polygon

================================================================================
MSG: geometry_msgs/Polygon
#A specification of a polygon where the first and last points are assumed to be connected
Point32[] points

================================================================================
MSG: geometry_msgs/Point32
# This contains the position of a point in free space(with 32 bits of precision).
# It is recommeded to use Point wherever possible instead of Point32.  
# 
# This recommendation is to promote interoperability.  
#
# This message is designed to take up less space when sending
# lots of points at once, as in the case of a PointCloud.  

float32 x
float32 y
float32 z
================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

")



(provide :swram_robot_mapping_tutorial/cluster_data "19a74e2a774e33127a923df0183079be")


