#!/usr/bin/env python3
import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    pkg_nav2 = get_package_share_directory('mec_mobile_nav2')
    nav2_params = os.path.join(pkg_nav2, 'config', 'nav2_params.yaml')

    return LaunchDescription([
        # SLAM Toolbox
        Node(
            package='slam_toolbox',
            executable='async_slam_toolbox_node',
            name='slam_toolbox',
            output='screen',
            parameters=[nav2_params]
        ),

        # Nav2 stack
        Node(
            package='nav2_controller',
            executable='controller_server',
            output='screen',
            parameters=[nav2_params]
        ),
        Node(
            package='nav2_planner',
            executable='planner_server',
            output='screen',
            parameters=[nav2_params]
        ),
        Node(
            package='nav2_bt_navigator',
            executable='bt_navigator',
            output='screen',
            parameters=[nav2_params]
        ),
        Node(
            package='nav2_smoother',
            executable='smoother_server',
            output='screen',
            parameters=[nav2_params]
        ),
        Node(
            package='nav2_velocity_smoother',
            executable='velocity_smoother',
            output='screen',
            parameters=[nav2_params]
        ),

        # Lifecycle manager – tự động bật hết
        Node(
            package='nav2_lifecycle_manager',
            executable='lifecycle_manager',
            name='lifecycle_manager_navigation',
            output='screen',
            parameters=[{'use_sim_time': True,
                         'autostart': True,
                         'node_names': ['controller_server',
                                        'planner_server',
                                        'bt_navigator',
                                        'smoother_server',
                                        'velocity_smoother']}]
        )
    ])