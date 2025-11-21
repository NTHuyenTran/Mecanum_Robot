#!/usr/bin/env python3
import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():
    # Đường dẫn đến file params của mình
    nav2_params = os.path.join(
        get_package_share_directory('mec_mobile_nav2'),
        'config',
        'nav2_params.yaml'
    )

    # Nav2 bringup chính thức
    nav2_bringup_launch = os.path.join(
        get_package_share_directory('nav2_bringup'),
        'launch',
        'navigation_launch.py'
    )

    bt_xml = os.path.join(
    get_package_share_directory('nav2_bt_navigator'),
    'behavior_trees',
    'navigate_to_pose_w_replanning_and_recovery.xml'
    )


    return LaunchDescription([
        # SLAM Toolbox
        Node(
            package='slam_toolbox',
            executable='async_slam_toolbox_node',
            name='slam_toolbox',
            output='screen',
            parameters=[nav2_params, {'use_sim_time': True}]
        ),

        # Nav2 full
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(nav2_bringup_launch),
            launch_arguments={
                'params_file': nav2_params,
                'use_sim_time': 'true',
                'autostart': 'true',
                'default_nav_to_pose_bt_xml': bt_xml,
            }.items(),
        ),
    ])