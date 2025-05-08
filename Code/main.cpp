// main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <limits> // 需要用于 std::numeric_limits

// 包含所有管理器和核心类头文件
#include "../include/User.hpp"
#include "../include/Resource.hpp"
#include "../include/Rental.hpp"
#include "../include/Billing.hpp"
#include "../include/Notification.hpp"
#include "../include/DataManager.hpp"

// 管理器类实例的占位符
// UserManager userManager;
// ResourceManager resourceManager;
// RentalManager rentalManager;
// BillingManager billingManager;
// NotificationManager notificationManager;
// DataManager dataManager;

// 当前登录用户的占位符
// User* currentUser = nullptr;

/**
 * @brief 显示应用程序的主菜单。
 * 
 * 根据用户的角色（如果已登录）或未登录状态，
 * 显示不同的选项。
 */
void displayMainMenu() {
    std::cout << "\n===== 云计算资源租赁系统 =====" << std::endl;
    // if (currentUser == nullptr) {
    //     std::cout << "1. 登录" << std::endl;
    //     std::cout << "2. 注册学生账号" << std::endl;
    //     std::cout << "3. 注册教师账号" << std::endl;
    //     std::cout << "0. 退出" << std::endl;
    // } else {
    //     std::cout << "已登录为：" << currentUser->getUsername() << " (" << /* UserRoleToString(currentUser->getRole()) */ ")" << std::endl;
    //     // currentUser->displayDashboard(); // 多态调用以显示角色特定菜单
    //     std::cout << "9. 登出" << std::endl;
    //     std::cout << "0. 退出并保存数据" << std::endl;
    // }
    std::cout << "请输入您的选择：";
}

/**
 * @brief 处理登录流程。
 * 
 * 提示输入用户名和密码，使用UserManager验证凭据。
 * 如果登录成功则设置currentUser。
 */
void handleLogin() {
    // std::string username, password;
    // std::cout << "请输入用户名：";
    // std::cin >> username;
    // std::cout << "请输入密码：";
    // std::cin >> password; // 实现安全的密码输入（例如，不回显）
    
    // currentUser = userManager.login(username, password);
    // if (currentUser) {
    //     std::cout << "登录成功！" << std::endl;
    //     // notificationManager.displayUserNotifications(currentUser->getUserId()); // 显示未读通知
    // } else {
    //     std::cout << "登录失败。用户名或密码无效。" << std::endl;
    // }
}

/**
 * @brief 处理新用户（学生/教师）的注册流程。
 * @param role 要注册的用户角色（STUDENT或TEACHER）。
 */
void handleRegistration(UserRole role) {
    // std::string username, password, confirmPassword;
    // std::cout << "请输入期望的用户名：";
    // std::cin >> username;
    // std::cout << "请输入密码：";
    // std::cin >> password; // 实现安全的密码输入
    // std::cout << "确认密码：";
    // std::cin >> confirmPassword;

    // if (password != confirmPassword) {
    //     std::cout << "密码不匹配。注册失败。" << std::endl;
    //     return;
    // }

    // bool success = userManager.registerUser(username, password, role);
    // if (success) {
    //     std::cout << "注册成功！您现在可以登录了。" << std::endl;
    // } else {
    //     std::cout << "注册失败。用户名可能已存在或出现其他错误。" << std::endl;
    // }
}

/**
 * @brief 主函数 - 应用程序入口点。
 *
 * 初始化系统，加载数据，处理主应用程序循环，
 * 并在退出时保存数据。
 */
int main() {
    // 初始化管理器
    // dataManager = DataManager();
    // userManager = UserManager(dataManager);
    // resourceManager = ResourceManager(dataManager);
    // rentalManager = RentalManager(dataManager, userManager, resourceManager, notificationManager);
    // billingManager = BillingManager(dataManager, userManager);
    // notificationManager = NotificationManager(dataManager);

    // 从文件加载数据
    // std::cout << "正在加载系统数据..." << std::endl;
    // userManager.loadUsers();
    // resourceManager.loadResources();
    // rentalManager.loadRentals();
    // billingManager.loadBillingData();
    // notificationManager.loadNotifications();
    // std::cout << "数据加载完成。" << std::endl;

    int choice;
    bool running = true;

    while (running) {
        displayMainMenu();
        std::cin >> choice;

        // 输入验证
        if (std::cin.fail()) {
            std::cin.clear(); // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 丢弃无效输入
            std::cout << "输入无效。请输入一个数字。" << std::endl;
            continue;
        }

        // if (currentUser == nullptr) {
        //     switch (choice) {
        //         case 1: // 登录
        //             handleLogin();
        //             break;
        //         case 2: // 注册学生
        //             handleRegistration(UserRole::STUDENT);
        //             break;
        //         case 3: // 注册教师
        //             handleRegistration(UserRole::TEACHER);
        //             break;
        //         case 0: // 退出
        //             running = false;
        //             break;
        //         default:
        //             std::cout << "选择无效。请重试。" << std::endl;
        //     }
        // } else {
        //     // 处理已登录用户的选择（委托给用户的displayDashboard或特定处理程序）
        //     // 这部分将根据角色和选择调用currentUser或特定管理器的方法
        //     if (choice == 9) { // 登出
        //         std::cout << "正在登出 " << currentUser->getUsername() << std::endl;
        //         currentUser = nullptr;
        //     } else if (choice == 0) { // 退出并保存
        //         running = false;
        //     } else {
        //         // 将选择传递给使用currentUser->getRole()进行委托的处理函数
        //         // 例如：handleUserChoice(currentUser, choice);
        //         std::cout << "此角色或选择的功能尚未实现。" << std::endl;
        //     }
        // }
        if (choice == 0) running = false; // 框架简化的退出
        else std::cout << "选择的占位符：" << choice << std::endl;
    }

    // 退出前保存数据到文件
    // std::cout << "正在保存系统数据..." << std::endl;
    // userManager.saveUsers();
    // resourceManager.saveResources();
    // rentalManager.saveRentals();
    // billingManager.saveBillingData();
    // notificationManager.saveNotifications();
    // std::cout << "数据保存完成。正在退出系统。" << std::endl;

    std::cout << "正在退出系统。" << std::endl;
    return 0;
}