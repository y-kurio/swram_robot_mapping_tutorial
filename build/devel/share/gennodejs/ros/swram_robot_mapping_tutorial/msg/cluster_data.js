// Auto-generated. Do not edit!

// (in-package swram_robot_mapping_tutorial.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let geometry_msgs = _finder('geometry_msgs');
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class cluster_data {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.cluster_number = null;
      this.header = null;
      this.cluster_type = null;
      this.cluster_points = null;
      this.orientation = null;
    }
    else {
      if (initObj.hasOwnProperty('cluster_number')) {
        this.cluster_number = initObj.cluster_number
      }
      else {
        this.cluster_number = [];
      }
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('cluster_type')) {
        this.cluster_type = initObj.cluster_type
      }
      else {
        this.cluster_type = [];
      }
      if (initObj.hasOwnProperty('cluster_points')) {
        this.cluster_points = initObj.cluster_points
      }
      else {
        this.cluster_points = [];
      }
      if (initObj.hasOwnProperty('orientation')) {
        this.orientation = initObj.orientation
      }
      else {
        this.orientation = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type cluster_data
    // Serialize message field [cluster_number]
    bufferOffset = _arraySerializer.int32(obj.cluster_number, buffer, bufferOffset, null);
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [cluster_type]
    bufferOffset = _arraySerializer.float32(obj.cluster_type, buffer, bufferOffset, null);
    // Serialize message field [cluster_points]
    // Serialize the length for message field [cluster_points]
    bufferOffset = _serializer.uint32(obj.cluster_points.length, buffer, bufferOffset);
    obj.cluster_points.forEach((val) => {
      bufferOffset = geometry_msgs.msg.PolygonStamped.serialize(val, buffer, bufferOffset);
    });
    // Serialize message field [orientation]
    // Serialize the length for message field [orientation]
    bufferOffset = _serializer.uint32(obj.orientation.length, buffer, bufferOffset);
    obj.orientation.forEach((val) => {
      bufferOffset = geometry_msgs.msg.Point.serialize(val, buffer, bufferOffset);
    });
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type cluster_data
    let len;
    let data = new cluster_data(null);
    // Deserialize message field [cluster_number]
    data.cluster_number = _arrayDeserializer.int32(buffer, bufferOffset, null)
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [cluster_type]
    data.cluster_type = _arrayDeserializer.float32(buffer, bufferOffset, null)
    // Deserialize message field [cluster_points]
    // Deserialize array length for message field [cluster_points]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.cluster_points = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.cluster_points[i] = geometry_msgs.msg.PolygonStamped.deserialize(buffer, bufferOffset)
    }
    // Deserialize message field [orientation]
    // Deserialize array length for message field [orientation]
    len = _deserializer.uint32(buffer, bufferOffset);
    data.orientation = new Array(len);
    for (let i = 0; i < len; ++i) {
      data.orientation[i] = geometry_msgs.msg.Point.deserialize(buffer, bufferOffset)
    }
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.cluster_number.length;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    length += 4 * object.cluster_type.length;
    object.cluster_points.forEach((val) => {
      length += geometry_msgs.msg.PolygonStamped.getMessageSize(val);
    });
    length += 24 * object.orientation.length;
    return length + 16;
  }

  static datatype() {
    // Returns string type for a message object
    return 'swram_robot_mapping_tutorial/cluster_data';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '19a74e2a774e33127a923df0183079be';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32[]  cluster_number
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new cluster_data(null);
    if (msg.cluster_number !== undefined) {
      resolved.cluster_number = msg.cluster_number;
    }
    else {
      resolved.cluster_number = []
    }

    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.cluster_type !== undefined) {
      resolved.cluster_type = msg.cluster_type;
    }
    else {
      resolved.cluster_type = []
    }

    if (msg.cluster_points !== undefined) {
      resolved.cluster_points = new Array(msg.cluster_points.length);
      for (let i = 0; i < resolved.cluster_points.length; ++i) {
        resolved.cluster_points[i] = geometry_msgs.msg.PolygonStamped.Resolve(msg.cluster_points[i]);
      }
    }
    else {
      resolved.cluster_points = []
    }

    if (msg.orientation !== undefined) {
      resolved.orientation = new Array(msg.orientation.length);
      for (let i = 0; i < resolved.orientation.length; ++i) {
        resolved.orientation[i] = geometry_msgs.msg.Point.Resolve(msg.orientation[i]);
      }
    }
    else {
      resolved.orientation = []
    }

    return resolved;
    }
};

module.exports = cluster_data;
