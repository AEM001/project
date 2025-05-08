// User.hpp
#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
// #include "Rental.hpp" // 后续如需租赁历史记录，可前向声明或包含

// 用户角色枚举
enum class UserRole {
    STUDENT,
    TEACHER,
    ADMIN
};

// 用户状态枚举
enum class UserStatus {
    ACTIVE,
    SUSPENDED // 因违规被管理员暂停使用
};

/**
 * @class User
 * @brief 系统中所有用户的基类。
 *
 * 存储用户通用信息，如ID、用户名、密码、角色和状态。
 * 提供基本功能如登录、个人资料管理和余额查询。
 * 交互对象：
 *  - 租赁模块：查看租赁历史。
 *  - 账单模块：查看账户余额。
 *  - 通知模块：接收系统消息。
 */
class User {
protected:
    std::string userId;
    std::string username;
    std::string Password; 
    double accountBalance;
    UserStatus status;
    // std::vector<RentalRecord> rentalHistory; // 可选，或由租赁系统管理

public:
    // 构造函数
    User(std::string id, std::string name, std::string password,  double balance = 0.0, UserStatus stat = UserStatus::ACTIVE):userId(id),username(name),Password(password){}
    virtual ~User() = default;

    // 获取器
    std::string getUserId() const{return userId;}
    std::string getUsername() const{return username;}
    UserRole getRole() const{};
    double getAccountBalance() const{return accountBalance;}
    UserStatus getStatus() const{return status;};

    // 设置器
    void setUsername(const std::string& newName){username=newName;}
    void setPassword(const std::string& newPassword){Password=newPassword;} // 密码修改逻辑
    void setStatus(UserStatus newStatus){status=newStatus;}

    // 账户余额管理
    virtual void deposit(double amount);
    virtual bool withdraw(double amount); // 成功返回true

    // 身份验证
    bool verifyPassword(const std::string& password) const
    {return password==Password;}

    // 显示用户特定菜单/信息的虚函数
    void displayDashboard() const{
        std::cout<<"ID:"<<userId<<std::endl<<"name:"<<username<<std::endl<<"当前余额为:"<<accountBalance<<std::endl<<"状态"<<status<<std::endl;
    }

    // 序列化/反序列化方法（用于数据持久化）
    // virtual void serialize(std::ostream& os) const;
    // virtual void deserialize(std::istream& is);
};

/**
 * @class Student
 * @brief 表示学生用户。
 *
 * 继承自User。可能具有特定的配额或优先级。
 * 交互对象：
 *  - 资源模块：浏览和租用资源。
 *  - 租赁模块：管理其租赁。
 */
class Student : public User {
public:
    Student(std::string id, std::string name, std::string password):User(id,name,password){}

    void deposit(double amount) override{
        // 还需要增加异常处理
        if(amount>=0)accountBalance+=amount;

    };


};

/**
 * @class Teacher
 * @brief 表示教师用户。
 *
 * 继承自User。可能具有更高的资源分配优先级。
 * 交互对象：
 *  - 资源模块：浏览和租用资源（可能具有更高优先级）。
 *  - 租赁模块：管理其租赁。
 */
class Teacher : public User {
public:
    Teacher(std::string id, std::string name, std::string password, double balance = 0.0);

};

/**
 * @class Admin
 * @brief 表示系统管理员。
 *
 * 继承自User。具有管理用户、资源和审核租赁请求的权限。
 * 管理员账户是预设的或由其他管理员添加的。
 * 交互对象：
 *  - 用户模块：管理所有用户（添加、修改、更改状态）。
 *  - 资源模块：管理资源（添加、修改、删除）。
 *  - 租赁模块：批准/拒绝租赁请求，查看所有租赁。
 *  - 账单模块：设置计费标准，查看所有账单。
 */
class Admin : public User {
public:
    Admin(std::string id, std::string name, std::string password);

    void displayDashboard() const override;

    // 管理员特定功能
    // void manageUser(User& user, UserStatus newStatus);
    // void addResource(/* 资源详情 */);
    // void modifyResource(/* 资源详情 */);
    // void deleteResource(/* 资源ID */);
    // void approveRentalRequest(/* 租赁请求ID */);
    // void rejectRentalRequest(/* 租赁请求ID */);
    // void setBillingRate(/* 资源类型, 新费率 */);
};

#endif // USER_HPP