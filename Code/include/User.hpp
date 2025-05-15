#ifndef USER_HPP
#define USER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <fstream> // 添加文件流头文件
#include "Resource.hpp"
// #include "Rental.hpp" // 后续如需租赁历史记录，可前向声明或包含

// 还需要解决初始化自动编号的问题

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
// 枚举转字符串函数
const char* UserRoleToString(UserRole role) {
    switch(role) {
        case UserRole::STUDENT: return "学生";
        case UserRole::TEACHER: return "教师";
        case UserRole::ADMIN: return "管理员";
        default: return "未知角色";
    }
}
const char* UserStatusToString(UserStatus status) {
    switch(status) {
        case UserStatus::ACTIVE: return "正常";
        case UserStatus::SUSPENDED: return "暂停";
        default: return "未知状态";
    }
}
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
    UserRole role;
    // std::vector<RentalRecord> rentalHistory; // 可选，或由租赁系统管理

public:
    // 构造函数
    User(std::string id, std::string name, std::string password,  double balance = 0.0, UserRole r,UserStatus stat = UserStatus::ACTIVE)
        : userId(id), username(name), Password(password), accountBalance(balance),role(r), status(stat) {}
    virtual ~User() = default;

    // 获取器
    std::string getUserId() const{return userId;}
    std::string getUsername() const{return username;}
    virtual UserRole getRole() const=0;
    double getAccountBalance() const{return accountBalance;}
    UserStatus getStatus() const{return status;};

    // 设置器
    void setUsername(const std::string& newName){username=newName;}
    void setPassword(const std::string& newPassword){Password=newPassword;} // 密码修改逻辑
    void setStatus(UserStatus newStatus){status=newStatus;}

    // 账户余额管理
    virtual void deposit(double amount)=0;
    virtual bool withdraw(double amount)=0; // 成功返回true

   
    // 身份验证
    bool verifyPassword(const std::string& password) const
    {return password==Password;}

    // 显示用户特定菜单/信息的纯虚函数
    virtual void displayDashboard() const=0;
    // 序列化/反序列化方法（用于数据持久化）
    virtual void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(userId.c_str(), userId.size() + 1);
        os.write(username.c_str(), username.size() + 1);
        os.write(Password.c_str(), Password.size() + 1);
        os.write(reinterpret_cast<const char*>(&accountBalance), sizeof(double));
        os.write(reinterpret_cast<const char*>(&status), sizeof(UserStatus));
    }
    
    virtual void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        userId = buffer;
        
        is.getline(buffer, 256, '\0');
        username = buffer;
        
        is.getline(buffer, 256, '\0');
        Password = buffer;
        
        is.read(reinterpret_cast<char*>(&accountBalance), sizeof(double));
        is.read(reinterpret_cast<char*>(&status), sizeof(UserStatus));
    }
};

/**
 * 继承自User。具有特定的配额或优先级。
 * 交互对象：
 *  - 资源模块：浏览和租用资源。
 *  - 租赁模块：管理其租赁。
 */
class Student : public User {
public:
    Student(std::string id, std::string name, std::string password,UserRole role=UserRole::STUDENT)
         : User(id, name, password) {} 

    
    // 空构造函数用于反序列化
    Student() : User("", "", "") {}
    
    UserRole getRole() const override{return UserRole::STUDENT;}
    
    void deposit(double amount) override{
        // 还需要增加异常处理
        if(amount>=0)accountBalance+=amount;

    };
    bool withdraw(double amount) override{
        // 还需要增加异常处理
        if(amount>=0&&amount<=accountBalance){
            accountBalance-=amount;
            return true;
        }
        return false;

    }
    void displayDashboard() const override {
        std::cout << "===== 学生控制面板 =====\n";
        std::cout << "ID: " << userId << "\n";
        std::cout << "用户名: " << username << "\n";
        std::cout << "当前余额: " << accountBalance << "\n";
        std::cout << "状态: " << (status == UserStatus::ACTIVE? "活跃" : "已暂停") << "\n";
        std::cout << "可用功能:\n";
        std::cout << "1. 浏览资源\n";
        std::cout << "2. 租用资源\n";
        std::cout << "3. 查看租赁历史\n";
        std::cout << "4. 查看账户余额\n";
        std::cout << "5. 退出\n";
    }


    // 序列化/反序列化方法
    void serialize(std::ostream& os) const override {
        // 先序列化基类部分
        User::serialize(os);
        // 学生类没有额外属性需要序列化
    }
    
    void deserialize(std::istream& is) override {
        // 反序列化基类部分
        User::deserialize(is);
        // 学生类没有额外属性需要反序列化
    }
};

/**
 * 继承自User。可能具有更高的资源分配优先级。
 * 交互对象：
 *  - 资源模块：浏览和租用资源（具有更高优先级）。
 *  - 租赁模块：管理其租赁。
 */
class Teacher : public User {
public:
    Teacher(std::string id, std::string name, std::string password, double balance = 0.0,UserRole role=UserRole::TEACHER)
        : User(id, name, password, balance) {}
    
    // 空构造函数用于反序列化
    Teacher() : User("", "", "") {}
    
    UserRole getRole() const override{return UserRole::TEACHER;}
    
    void deposit(double amount) override{
        // 还需要增加异常处理
        if(amount>=0)accountBalance+=amount;
    };
    bool withdraw(double amount) override{
        // 还需要增加异常处理
        if(amount>=0&&amount<=accountBalance){
            accountBalance-=amount;
            return true;
        }
        return false;
    }
    void displayDashboard() const override {
        std::cout << "===== 教师控制面板 =====\n";
        std::cout << "ID: " << userId << "\n";
        std::cout << "用户名: " << username << "\n";
        std::cout << "当前余额: " << accountBalance << "\n";
        std::cout << "状态: " << (status == UserStatus::ACTIVE? "活跃" : "已暂停") << "\n";
        std::cout << "可用功能:\n"; 
        std::cout << "1. 浏览资源\n";
        std::cout << "2. 租用资源\n";
        std::cout << "3. 查看租赁历史\n";
        std::cout << "4. 查看账户余额\n";
        std::cout << "5. 退出\n";
    }

    // 序列化/反序列化方法
    void serialize(std::ostream& os) const override {
        // 先序列化基类部分
        User::serialize(os);
        // 教师类没有额外属性需要序列化
    }
    
    void deserialize(std::istream& is) override {
        // 反序列化基类部分
        User::deserialize(is);
        // 教师类没有额外属性需要反序列化
    }
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
    Admin(std::string id, std::string name, std::string password)
        : User(id, name, password) {}
    
    // 空构造函数用于反序列化
    Admin() : User("", "", "") {}
    
    UserRole getRole() const override { return UserRole::ADMIN; }
    
    void displayDashboard() const override {
        std::cout << "===== 管理员控制面板 =====\n";
        std::cout << "ID: " << userId << "\n";
        std::cout << "用户名: " << username << "\n";
        std::cout << "当前余额: " << accountBalance << "\n";
        std::cout << "状态: " << (status == UserStatus::ACTIVE ? "活跃" : "已暂停") << "\n";
        std::cout << "可用功能:\n";
        std::cout << "1. 管理用户\n";
        std::cout << "2. 管理资源\n";
        std::cout << "3. 管理租赁请求\n";
        std::cout << "4. 设置计费标准\n";
    }

    // 管理员特定功能
    void manageUser(User& user, UserStatus newStatus) {
        user.setStatus(newStatus);
        std::cout << "用户 " << user.getUsername() << " 状态已更新为 " 
                  << (newStatus == UserStatus::ACTIVE ? "活跃" : "已暂停") << std::endl;
    }
    
    // 资源管理功能
    void addResource(ResourceCollection& collection, std::shared_ptr<Resource> resource) {
        collection.addResource(resource);
        std::cout << "已添加新资源: " << resource->getResourceName() << " (ID: " << resource->getResourceId() << ")" << std::endl;
        
        // 保存更新后的资源集合到文件
        try {
            collection.saveToFile("resources.dat");
            std::cout << "资源数据已保存到文件" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "保存资源数据失败: " << e.what() << std::endl;
        }
    }
    
    void modifyResource(ResourceCollection& collection, const std::string& resourceId, 
                        const std::string& newName, double newRate) {
        auto resource = collection.findResourceById(resourceId);
        if (resource) {
            resource->setResourceName(newName);
            resource->setHourlyRate(newRate);
            std::cout << "资源 " << resourceId << " 已更新" << std::endl;
            
            // 保存更新后的资源集合到文件
            try {
                collection.saveToFile("resources.dat");
                std::cout << "资源数据已保存到文件" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "保存资源数据失败: " << e.what() << std::endl;
            }
        } else {
            std::cout << "未找到资源 " << resourceId << std::endl;
        }
    }
    
    void deleteResource(ResourceCollection& collection, const std::string& resourceId) {
        auto& resources = const_cast<std::vector<std::shared_ptr<Resource>>&>(collection.getAllResources());
        for (auto it = resources.begin(); it != resources.end(); ++it) {
            if ((*it)->getResourceId() == resourceId) {
                resources.erase(it);
                std::cout << "资源 " << resourceId << " 已删除" << std::endl;
                
                // 保存更新后的资源集合到文件
                try {
                    collection.saveToFile("resources.dat");
                    std::cout << "资源数据已保存到文件" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "保存资源数据失败: " << e.what() << std::endl;
                }
                return;
            }
        }
        std::cout << "未找到资源 " << resourceId << std::endl;
    }
    
    void loadResourceData(ResourceCollection& collection) {
        try {
            collection.loadFromFile("resources.dat");
            std::cout << "已从文件加载资源数据" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "加载资源数据失败: " << e.what() << std::endl;
        }
    }
    
    void setBillingRate(ResourceCollection& collection, ResourceType type, double newRate) {
        auto resources = collection.getResourcesByType(type);
        for (auto& resource : resources) {
            resource->setHourlyRate(newRate);
        }
        std::cout << "已更新所有 " << (type == ResourceType::CPU ? "CPU" : "GPU") 
                  << " 资源的计费标准为 " << newRate << " 元/小时" << std::endl;
        
        // 保存更新后的资源集合到文件
        try {
            collection.saveToFile("resources.dat");
            std::cout << "资源数据已保存到文件" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "保存资源数据失败: " << e.what() << std::endl;
        }
    }
    
    // 租赁请求管理功能
    void approveRentalRequest(const std::string& requestId) {
        // 实现租赁请求批准逻辑
        std::cout << "已批准租赁请求 " << requestId << std::endl;
    }
    
    void rejectRentalRequest(const std::string& requestId) {
        // 实现租赁请求拒绝逻辑
        std::cout << "已拒绝租赁请求 " << requestId << std::endl;
    }
};



/**
 * @class UserCollection
 * @brief 管理系统中所有用户的集合。
 *
 * 提供添加、查找、列出用户的功能。
 * 可以按角色筛选用户。
 */
class UserCollection {
private:
    std::vector<std::shared_ptr<User>> users;

public:
    // 添加用户到集合
    void addUser(std::shared_ptr<User> user) {
        users.push_back(user);
    }

    // 根据ID查找用户
    std::shared_ptr<User> findUserById(const std::string& id) const {
        for (const auto& user : users) {
            if (user->getUserId() == id) {
                return user;
            }
        }
        return nullptr; // 未找到用户
    }

    // 获取所有用户
    const std::vector<std::shared_ptr<User>>& getAllUsers() const {
        return users;
    }

    // 获取特定角色的用户
    std::vector<std::shared_ptr<User>> getUsersByRole(UserRole role) const {
        std::vector<std::shared_ptr<User>> result;
        for (const auto& user : users) {
            if (user->getRole() == role) {
                result.push_back(user);
            }
        }
        return result;
    }

    // 显示所有用户
    void displayAllUsers() const {
        std::cout << "===== 所有用户列表 =====\n";
        for (const auto& user : users) {
            std::cout << "ID: " << user->getUserId() << "\n";
            std::cout << "用户名: " << user->getUsername() << "\n";
            std::cout << "角色: ";
            switch (user->getRole()) {
                case UserRole::STUDENT: std::cout << "学生\n"; break;
                case UserRole::TEACHER: std::cout << "教师\n"; break;
                case UserRole::ADMIN: std::cout << "管理员\n"; break;
            }
            std::cout << "状态: " << (user->getStatus() == UserStatus::ACTIVE ? "活跃" : "已暂停") << "\n";
            std::cout << "余额: " << user->getAccountBalance() << "\n";
            std::cout << "------------------------\n";
        }
    }

    // 持久化方法
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + filename);
        }
        
        // 写入用户数量
        size_t count = users.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入用户
        for (const auto& user : users) {
            // 写入用户角色标识
            UserRole role = user->getRole();
            file.write(reinterpret_cast<const char*>(&role), sizeof(UserRole));
            
            // 根据角色序列化用户
            user->serialize(file);
        }
        
        file.close();
    }
    
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行读取: " + filename);
        }
        
        // 清空当前用户
        users.clear();
        
        // 读取用户数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取用户
        for (size_t i = 0; i < count; ++i) {
            // 读取用户角色标识
            UserRole role;
            file.read(reinterpret_cast<char*>(&role), sizeof(UserRole));
            
            // 根据角色创建相应的用户对象
            std::shared_ptr<User> user;
            switch (role) {
                case UserRole::STUDENT:
                    user = std::make_shared<Student>();
                    break;
                case UserRole::TEACHER:
                    user = std::make_shared<Teacher>();
                    break;
                case UserRole::ADMIN:
                    user = std::make_shared<Admin>();
                    break;
                default:
                    throw std::runtime_error("未知的用户角色");
            }
            
            // 反序列化用户
            user->deserialize(file);
            
            // 添加到集合
            users.push_back(user);
        }
        
        file.close();
    }
};

// 创建预设用户集合的辅助函数
inline UserCollection createDefaultUserCollection() {
    UserCollection collection;
    
    // 添加管理员用户
    collection.addUser(std::make_shared<Admin>("admin001", "系统管理员", "admin123"));
    
    // 添加教师用户
    collection.addUser(std::make_shared<Teacher>("teacher001", "张教授", "teacher123", 1000.0));
    collection.addUser(std::make_shared<Teacher>("teacher002", "李教授", "teacher123", 1000.0));
    collection.addUser(std::make_shared<Teacher>("teacher003", "王教授", "teacher123", 1000.0));
    
    // 添加学生用户
    collection.addUser(std::make_shared<Student>("student001", "张三", "student123"));
    collection.addUser(std::make_shared<Student>("student002", "李四", "student123"));
    collection.addUser(std::make_shared<Student>("student003", "王五", "student123"));
    collection.addUser(std::make_shared<Student>("student004", "赵六", "student123"));
    collection.addUser(std::make_shared<Student>("student005", "钱七", "student123"));
    
    return collection;
}

// 修正这里
#endif // USER_HPP
