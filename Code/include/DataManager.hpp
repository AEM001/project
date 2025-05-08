// DataManager.hpp
#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include <string>
#include <vector>
// 前向声明或包含需要持久化的必要类
// class User;
// class Resource;
// struct RentalRecord;
// struct Bill; 
// struct LogEntry; // 假设有用于日志的 LogEntry 结构体/类

/**
 * @class DataManager
 * @brief 处理应用程序数据的加载和保存到持久化存储。
 *
 * 管理用户、资源、租赁记录、账单和日志的持久化，
 * 按照需求指定的格式（例如：users.dat, resources.dat）。
 * 交互对象：
 *  - 所有管理器类（UserManager、ResourceManager、RentalManager、BillingManager）用于获取/设置数据。
 *  - 如果日志比较复杂，可能需要日志模块。
 */
class DataManager {
private:
    const std::string usersFile = "users.dat";
    const std::string resourcesFile = "resources.dat";
    const std::string rentalsFile = "rentals.dat"; // 可能需要分为请求和记录两部分
    const std::string billsFile = "bills.dat";
    const std::string logsFile = "logs.dat";

public:
    DataManager();

    // 由于不同对象结构的差异，通用的加载/保存可能过于复杂。
    // 为每种数据类型提供特定的加载/保存方法更实用。

    // 示例：用户数据持久化
    // std::vector<User> loadUsers();
    // bool saveUsers(const std::vector<User>& users);

    // 示例：资源数据持久化
    // std::vector<std::unique_ptr<Resource>> loadResources(); // 使用 unique_ptr 实现多态
    // bool saveResources(const std::vector<std::unique_ptr<Resource>>& resources);

    // 示例：租赁数据持久化
    // std::vector<RentalRecord> loadRentalRecords();
    // bool saveRentalRecords(const std::vector<RentalRecord>& records);
    // std::vector<RentalRequest> loadRentalRequests(); 
    // bool saveRentalRequests(const std::vector<RentalRequest>& requests);

    // 示例：账单数据持久化
    // std::vector<Bill> loadBills();
    // bool saveBills(const std::vector<Bill>& bills);
    // std::vector<BillingRule> loadBillingRules();
    // bool saveBillingRules(const std::vector<BillingRule>& rules);

    // 示例：日志数据持久化
    // std::vector<LogEntry> loadLogs();
    // bool saveLog(const LogEntry& logEntry); // 或者一次性保存所有日志

    // 备份机制
    // bool performBackup();

    // 用于生成唯一ID的工具函数，可以在这里实现或在各自的管理器中实现
    // std::string generateUniqueId(); 
};

#endif // DATAMANAGER_HPP