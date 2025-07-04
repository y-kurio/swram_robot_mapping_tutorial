
(cl:in-package :asdf)

(defsystem "swram_robot_mapping_tutorial-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "cluster_data" :depends-on ("_package_cluster_data"))
    (:file "_package_cluster_data" :depends-on ("_package"))
  ))