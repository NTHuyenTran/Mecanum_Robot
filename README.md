# 🤖 Mecanum Robot Simulation (ROS2 Jazzy)

![ROS2](https://img.shields.io/badge/ROS2-Jazzy-blue?logo=ros)
![OS](https://img.shields.io/badge/OS-Ubuntu%2024.04-orange?logo=ubuntu)
![Status](https://img.shields.io/badge/Status-Work_in_Progress-yellow)

## 📌 About The Project

This repository is my personal project for learning and practicing **ROS2 Jazzy** on **Ubuntu 24.04**. 
It contains the source code to build, simulate, and control a 4-wheel Mecanum Robot. The project utilizes **Gazebo** for physical simulation and **RViz2** for visualization. It is developed using VSCode.

Currently, the basic robot model and simulation environments are set up. I am actively working on integrating SLAM (Mapping) and Autonomous Navigation (Nav2).

## ✨ Features & Roadmap

- [x] **Robot Modeling:** URDF / Xacro setup for the Mecanum robot.
- [x] **Visualization:** RViz2 integration for viewing the robot model and TF tree.
- [x] **Simulation:** Physics and environment simulation in Gazebo.
- [x] **Teleoperation:** Basic movement control (cmd_vel).
- [ ] **Mapping (SLAM):** Generating 2D maps of the environment (In Progress).
- [ ] **Navigation (Nav2):** Autonomous path planning and obstacle avoidance (Planned).

## 🛠️ Prerequisites

Before you begin, ensure you have met the following requirements:
* **OS:** Ubuntu 24.04
* **ROS2:** Jazzy Jalisco (Desktop Install)
* **Simulation:** Gazebo (compatible with ROS2 Jazzy)
* **Tools:** `colcon`, `git`

## 🚀 Installation & Setup

1. **Create a ROS2 workspace** (if you don't have one):
   ```bash
   mkdir -p ~/ros2_ws/src
   cd ~/ros2_ws/src

2.  **Clone the repository:**

    ```bash
    git clone [https://github.com/NTHuyenTran/Mecanum_Robot.git](https://github.com/NTHuyenTran/Mecanum_Robot.git)
    ```

3.  **Install dependencies:**

    ```bash
    cd ~/ros2_ws
    rosdep install --from-paths src --ignore-src -r -y
    ```

4.  **Build the project:**

    ```bash
    colcon build --symlink-install
    ```

5.  **Source the workspace:**

    ```bash
    source install/setup.bash
    ```

## 🎮 Usage

*(Note: Replace `[your_package_name]` and `[your_launch_file.py]` with your actual file names)*

**1. Launch the robot in RViz:**

```bash
ros2 launch [your_package_name] display.launch.py
```

**2. Launch the Gazebo simulation:**

```bash
ros2 launch [your_package_name] gazebo.launch.py
```

**3. Control the robot via keyboard:**

```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard
```

## 📂 Project Structure

```text
Mecanum_Robot/
├── description/        # URDF and Xacro files for the robot model
├── launch/             # Launch files for RViz and Gazebo
├── config/             # Configuration files (RViz, controllers, etc.)
├── worlds/             # Gazebo simulation environments
└── CMakeLists.txt / package.xml
```

## ✍️ Author

**NTHuyenTran**

  * GitHub: [@NTHuyenTran](https://www.google.com/search?q=https://github.com/NTHuyenTran)
