// Billing.hpp
#ifndef BILLING_HPP
#define BILLING_HPP

#include <string>
#include <vector>
#include <chrono>
#include "Resource.hpp" // 用于 ResourceType 枚举
// #include "User.hpp"
// #include "Rental.hpp"

// 前向声明
class User;
struct RentalRecord;

/**
 * @struct BillingRule
 * @brief 定义特定资源类型的计费率和规则。
 *
 * 管理员可以设置和调整这些规则。
 * 交互对象：
 *  - 管理员模块：用于设置和更新规则。
 *  - BillingManager：用于计算费用。
 */
struct BillingRule {
    ResourceType resourceType;
    double ratePerHour;         // 每小时费用
    // 潜在的其他规则，例如，最小计费单位（如1小时）
    // int minBillingUnitHours = 1; 

    BillingRule(ResourceType type, double rate);
};

/**
 * @struct Bill
 * @brief 表示已完成租赁的生成账单。
 *
 * 包含特定租赁记录的费用详情。
 * 交互对象：
 *  - 用户模块：供用户查看其账单。
 *  - 租赁模块：链接到特定的RentalRecord。
 */
struct Bill {
    std::string billId;
    std::string rentalId;       // 对应的租赁记录ID
    std::string userId;
    std::chrono::system_clock::time_point generationDate;
    double totalAmount;
    bool isPaid;

    Bill(std::string bId, std::string rId, std::string uId, double amount);
};

/**
 * @class BillingManager
 * @brief 管理计费规则，生成账单，并处理支付。
 *
 * 根据使用情况和计费规则计算费用，从用户余额中扣除。
 * 交互对象：
 *  - 用户模块：从用户余额扣除费用并通知余额不足。
 *  - 租赁模块：获取已完成的租赁记录以生成账单。
 *  - 管理员模块：供管理员设置计费规则和查看所有账单。
 *  - 数据持久化模块：保存/加载计费规则和账单。
 */
class BillingManager {
private:
    std::vector<BillingRule> billingRules;
    std::vector<Bill> generatedBills;
    // 潜在的账单唯一ID生成器

public:
    BillingManager();

    // 计费规则管理（管理员）
    // void setBillingRule(ResourceType type, double ratePerHour);
    // BillingRule getBillingRule(ResourceType type) const;
    // std::vector<BillingRule> getAllBillingRules() const;

    // 账单生成
    // Bill generateBillForRental(const RentalRecord& rental, const User& user);

    // 支付处理
    // bool processPayment(User& user, const Bill& bill); // 从用户余额中扣除

    // 账单查询
    // std::vector<Bill> getUserBills(const std::string& userId) const;
    // std::vector<Bill> getAllBills() const; // 供管理员使用

    // 持久化
    // void loadBillingData(const std::string& rulesFile, const std::string& billsFile);
    // void saveBillingData(const std::string& rulesFile, const std::string& billsFile) const;
};

#endif // BILLING_HPP