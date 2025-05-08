// Notification.hpp
#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <string>
#include <vector>
#include <chrono>

// 通知优先级枚举
enum class NotificationPriority {
    LOW,
    MEDIUM,
    HIGH // 用于重要提醒，如租赁超时、余额不足等
};

/**
 * @struct Notification
 * @brief 表示用户的系统通知。
 *
 * 包含消息详情、时间戳、优先级和阅读状态。
 * 交互对象：
 *  - 用户模块：用于向用户显示。
 *  - 通知管理器：用于创建和管理。
 */
struct Notification {
    std::string notificationId;
    std::string userId; // 通知接收用户的ID
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    NotificationPriority priority;
    bool isRead;

    // 构造函数
    Notification(std::string nId, std::string uId, std::string msg, NotificationPriority prio);
};

/**
 * @class NotificationManager
 * @brief 管理通知的创建、存储和检索。
 *
 * 处理各种系统事件的通知发送（如租赁状态变更、余额不足）。
 * 交互对象：
 *  - 用户模块：向特定用户发送通知。
 *  - 租赁模块：生成租赁事件的通知。
 *  - 账单模块：生成账单事件的通知（如余额不足）。
 *  - 数据持久化模块：保存/加载通知（通常通知可能更具临时性或以不同方式记录）。
 */
class NotificationManager {
private:
    std::vector<Notification> notifications;
    // 潜在的通知唯一ID生成器

public:
    NotificationManager();

    // 通知创建
    // void createNotification(const std::string& userId, const std::string& message, NotificationPriority priority);

    // 通知检索
    // std::vector<Notification> getUserNotifications(const std::string& userId, bool includeRead = false) const;
    // std::vector<Notification> getUnreadUserNotifications(const std::string& userId) const;

    // 通知管理
    // bool markNotificationAsRead(const std::string& notificationId);
    // void clearUserNotifications(const std::string& userId, bool clearOnlyRead = true);
    // void autoClearOldNotifications(); // 系统任务，用于清理旧的/已处理的通知

    // 持久化（通知的可选功能，可能作为日志的一部分）
    // void loadNotifications(const std::string& notificationsFile);
    // void saveNotifications(const std::string& notificationsFile) const;
};

#endif // NOTIFICATION_HPP