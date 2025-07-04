; Auto-generated. Do not edit!


(cl:in-package swram_robot_mapping_tutorial-msg)


;//! \htmlinclude cluster_data.msg.html

(cl:defclass <cluster_data> (roslisp-msg-protocol:ros-message)
  ((cluster_number
    :reader cluster_number
    :initarg :cluster_number
    :type (cl:vector cl:integer)
   :initform (cl:make-array 0 :element-type 'cl:integer :initial-element 0))
   (header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (cluster_type
    :reader cluster_type
    :initarg :cluster_type
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0))
   (cluster_points
    :reader cluster_points
    :initarg :cluster_points
    :type (cl:vector geometry_msgs-msg:PolygonStamped)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:PolygonStamped :initial-element (cl:make-instance 'geometry_msgs-msg:PolygonStamped)))
   (orientation
    :reader orientation
    :initarg :orientation
    :type (cl:vector geometry_msgs-msg:Point)
   :initform (cl:make-array 0 :element-type 'geometry_msgs-msg:Point :initial-element (cl:make-instance 'geometry_msgs-msg:Point))))
)

(cl:defclass cluster_data (<cluster_data>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <cluster_data>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'cluster_data)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name swram_robot_mapping_tutorial-msg:<cluster_data> is deprecated: use swram_robot_mapping_tutorial-msg:cluster_data instead.")))

(cl:ensure-generic-function 'cluster_number-val :lambda-list '(m))
(cl:defmethod cluster_number-val ((m <cluster_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader swram_robot_mapping_tutorial-msg:cluster_number-val is deprecated.  Use swram_robot_mapping_tutorial-msg:cluster_number instead.")
  (cluster_number m))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <cluster_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader swram_robot_mapping_tutorial-msg:header-val is deprecated.  Use swram_robot_mapping_tutorial-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'cluster_type-val :lambda-list '(m))
(cl:defmethod cluster_type-val ((m <cluster_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader swram_robot_mapping_tutorial-msg:cluster_type-val is deprecated.  Use swram_robot_mapping_tutorial-msg:cluster_type instead.")
  (cluster_type m))

(cl:ensure-generic-function 'cluster_points-val :lambda-list '(m))
(cl:defmethod cluster_points-val ((m <cluster_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader swram_robot_mapping_tutorial-msg:cluster_points-val is deprecated.  Use swram_robot_mapping_tutorial-msg:cluster_points instead.")
  (cluster_points m))

(cl:ensure-generic-function 'orientation-val :lambda-list '(m))
(cl:defmethod orientation-val ((m <cluster_data>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader swram_robot_mapping_tutorial-msg:orientation-val is deprecated.  Use swram_robot_mapping_tutorial-msg:orientation instead.")
  (orientation m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <cluster_data>) ostream)
  "Serializes a message object of type '<cluster_data>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'cluster_number))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let* ((signed ele) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    ))
   (cl:slot-value msg 'cluster_number))
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'cluster_type))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'cluster_type))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'cluster_points))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'cluster_points))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'orientation))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'orientation))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <cluster_data>) istream)
  "Deserializes a message object of type '<cluster_data>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'cluster_number) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'cluster_number)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296)))))))
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'cluster_type) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'cluster_type)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'cluster_points) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'cluster_points)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:PolygonStamped))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'orientation) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'orientation)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'geometry_msgs-msg:Point))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<cluster_data>)))
  "Returns string type for a message object of type '<cluster_data>"
  "swram_robot_mapping_tutorial/cluster_data")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'cluster_data)))
  "Returns string type for a message object of type 'cluster_data"
  "swram_robot_mapping_tutorial/cluster_data")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<cluster_data>)))
  "Returns md5sum for a message object of type '<cluster_data>"
  "19a74e2a774e33127a923df0183079be")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'cluster_data)))
  "Returns md5sum for a message object of type 'cluster_data"
  "19a74e2a774e33127a923df0183079be")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<cluster_data>)))
  "Returns full string definition for message of type '<cluster_data>"
  (cl:format cl:nil "int32[]  cluster_number~%std_msgs/Header header~%float32[] cluster_type~%geometry_msgs/PolygonStamped[] cluster_points~%geometry_msgs/Point[] orientation~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PolygonStamped~%# This represents a Polygon with reference coordinate frame and timestamp~%Header header~%Polygon polygon~%~%================================================================================~%MSG: geometry_msgs/Polygon~%#A specification of a polygon where the first and last points are assumed to be connected~%Point32[] points~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'cluster_data)))
  "Returns full string definition for message of type 'cluster_data"
  (cl:format cl:nil "int32[]  cluster_number~%std_msgs/Header header~%float32[] cluster_type~%geometry_msgs/PolygonStamped[] cluster_points~%geometry_msgs/Point[] orientation~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PolygonStamped~%# This represents a Polygon with reference coordinate frame and timestamp~%Header header~%Polygon polygon~%~%================================================================================~%MSG: geometry_msgs/Polygon~%#A specification of a polygon where the first and last points are assumed to be connected~%Point32[] points~%~%================================================================================~%MSG: geometry_msgs/Point32~%# This contains the position of a point in free space(with 32 bits of precision).~%# It is recommeded to use Point wherever possible instead of Point32.  ~%# ~%# This recommendation is to promote interoperability.  ~%#~%# This message is designed to take up less space when sending~%# lots of points at once, as in the case of a PointCloud.  ~%~%float32 x~%float32 y~%float32 z~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <cluster_data>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'cluster_number) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'cluster_type) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'cluster_points) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'orientation) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <cluster_data>))
  "Converts a ROS message object to a list"
  (cl:list 'cluster_data
    (cl:cons ':cluster_number (cluster_number msg))
    (cl:cons ':header (header msg))
    (cl:cons ':cluster_type (cluster_type msg))
    (cl:cons ':cluster_points (cluster_points msg))
    (cl:cons ':orientation (orientation msg))
))
