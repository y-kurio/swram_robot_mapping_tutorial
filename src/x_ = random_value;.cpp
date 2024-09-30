    x_ = random_value;
    y_ = sqrt(1-x^2);
    move_pose_x_ = pose_out.pose.position.x + x_;
    move_pose_y_ = pose_out.pose.position.x + y_;
    move_pose_ = sqrt(x_^2 + y_^2);
    pre_dis = sqrt((pose_out.pose.position.x - centerRow)^2 + (pose_out.pose.position.y - centerCol)^2);
    newposdis = sqrt((x_ - centerRow)^2 + (y_ - centerCol)^2);
    x_1_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((pre_dis - param2)^2) / (2 * (param1)^2));
    x_2_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((newposdis - param2)^2) / (2 * (param1)^2));
    SSS_ = x_2 / x_1;
            
    //  SSSの条件分岐
    if( SSS_ > random_value )
    {
        // ロボットの位置を更新
        goal = x_;
        currentpos_x = x_
    }
    else if
    {
        x_ = random;
        y_ = sqrt(1-x^2);
        move_pose_x_ = currentpos_x + x_;
        move_pose_y_ = currentpos_y + y_;
        move_pose_ = sqrt(x_^2 + y_^2);
        pre_dis = sqrt((pose_out.pose.position.x - centerRow)^2 + (pose_out.pose.position.y - centerCol)^2);
        newposdis = sqrt((x_ - centerRow)^2 + (y_ - centerCol)^2);
        x_1_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((pre_dis - param2)^2) / (2 * (param1)^2));
        x_2_ = (1 / sqrt(2 * 3.141592 * (param1)^2)) * exp(-((newposdis - param2)^2) / (2 * (param1)^2));
    }
        
    
                