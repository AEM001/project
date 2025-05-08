// Rental.hpp
#ifndef RENTAL_HPP
#define RENTAL_HPP

#include <string>
#include <vector>
#include <chrono> // 用于时间点
// #include "User.hpp"       // 如果需要存储直接的User对象，则前向声明或包含
// #include "Resource.hpp"   // 如果需要存储直接的Resource对象，则前向声明或包含

// 前向声明以避免头文件中不需要完整对象时的循环依赖
class User;
class Resource;

// 租赁状态枚举
enum class RentalStatus {
    PENDING_APPROVAL, // 用户已请求，等待管理员审批
    APPROVED,         // 管理员已批准，资源已分配（或将在开始时分配）
    REJECTED,         // 管理员拒绝了请求
    ACTIVE,           // 资源当前正在被用户使用
    COMPLETED,        // 租赁期已结束，资源已归还
    CANCELLED,        // 用户在开始前取消，或管理员取消
    EXPIRED           // 租赁请求在未经批准或操作的情况下过期
};

/**
 * @struct RentalRequest
 * @brief 表示用户租用资源的请求。
 *
 * 存储管理员批准或拒绝租赁所需的详细信息。
 * 交互对象：
 *  - 用户模块：将请求链接到用户。
 *  - 资源模块：指定所需的资源。
 *  - 管理员模块：用于审批流程。
 */
struct RentalRequest {
    std::string requestId;
    std::string userId;    // 发出请求的用户ID
    std::string resourceId; // 请求的资源ID
    std::chrono::system_clock::time_point requestTime;    // 发出请求的时间
    std::chrono::system_clock::time_point desiredStartTime; // 请求的租赁开始时间
    std::chrono::hours durationHours; // 请求的租赁时长
    RentalStatus status;          // 请求的当前状态
    std::string adminNotes;       // 管理员审核期间的可选注释

    // 构造函数
    RentalRequest(std::string reqId, std::string uId, std::string resId, 
                  std::chrono::system_clock::time_point startTime, std::chrono::hours duration);
};

/**
 * @struct RentalRecord
 * @brief 表示活动或已完成的租赁协议。
 *
 * 存储已确认租赁的详细信息，包括实际使用时间和费用。
 * 交互对象：
 *  - 用户模块：将租赁链接到用户。
 *  - 资源模块：链接到特定的租赁资源。
 *  - 账单模块：提供生成账单的数据。
 */
struct RentalRecord {
    std::string rentalId;         // 此租赁实例的唯一ID
    std::string requestId;        // 原始RentalRequest的ID
    std::string userId;
    std::string resourceId;
    std::chrono::system_clock::time_point actualStartTime;
    std::chrono::system_clock::time_point actualEndTime;
    double totalCost;
    RentalStatus status; // 例如：ACTIVE, COMPLETED

    // 构造函数
    RentalRecord(std::string rentId, std::string reqId, std::string uId, std::string resId, 
                 std::chrono::system_clock::time_point startTime);
    
    // 标记租赁完成并计算费用的方法
    // void completeRental(std::chrono::system_clock::time_point endTime, double cost);
};

/**
 * @class RentalManager
 * @brief 管理所有租赁请求和活动租赁。
 *
 * 处理租赁请求的创建、管理员审批/拒绝工作流、
 * 跟踪活动租赁和状态转换。
 * 交互对象：
 *  - 用户对象/管理器：验证用户状态、配额。
 *  - 资源对象/管理器：检查资源可用性、更新状态。
 *  - 管理员用户：用于租赁请求审批。
 *  - 账单模块：租赁完成后传递开票信息。
 *  - 通知模块：通知用户/管理员租赁状态变更。
 */
class RentalManager {
private:
    std::vector<RentalRequest> rentalRequests;
    std::vector<RentalRecord> activeRentals;
    std::vector<RentalRecord> rentalHistory; // 已完成或已取消的租赁
    // 潜在的请求/租赁唯一ID生成器

public:
    RentalManager();

    // 面向用户的操作
    // bool createRentalRequest(const User& user, const Resource& resource, std::chrono::system_clock::time_point startTime, std::chrono::hours duration);
    // bool cancelRentalRequest(const User& user, const std::string& requestId);
    // std::vector<RentalRequest> getUserRentalRequests(const std::string& userId) const;
    // std::vector<RentalRecord> getUserRentalHistory(const std::string& userId) const;

    // 面向管理员的操作
    // std::vector<RentalRequest> getPendingRentalRequests() const;
    // bool approveRentalRequest(const Admin& admin, const std::string& requestId, const std::string& notes = "");
    // bool rejectRentalRequest(const Admin& admin, const std::string& requestId, const std::string& notes = "");
    // std::vector<RentalRecord> getAllActiveRentals() const;
    // std::vector<RentalRecord> getAllRentalHistory() const;

    // 系统操作
    // void updateRentalStatuses(); // 例如：根据时间将状态从APPROVED改为ACTIVE，从ACTIVE改为COMPLETED
    // void processCompletedRental(const RentalRecord& record); // 通知账单系统，更新资源状态

    // 持久化
    // void loadRentalData(const std::string& requestFile, const std::string& recordFile);
    // void saveRentalData(const std::string& requestFile, const std::string& recordFile) const;
};

#endif // RENTAL_HPP