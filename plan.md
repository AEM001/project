
# 云算力资源租用系统 - 代码规划设计

## 一、核心类设计

### 1. 用户相关类

#### 1.1 基类：`User`（抽象基类）
- **属性**：
  - `string id` - 用户唯一标识
  - `string name` - 用户姓名
  - `string password` - 加密后的密码
  - `double balance` - 账户余额
  - `time_t registerTime` - 注册时间
  - `UserStatus status` - 用户状态（枚举类型）
- **方法**：
  - `bool verifyLogin(string password)` - 验证登录密码
  - `void changePassword(string oldPwd, string newPwd)` - 修改密码
  - `double getBalance()` - 查询余额
  - `void updateBalance(double amount)` - 更新余额
  - `virtual int calculateQuota() = 0` - 计算资源配额（纯虚函数）
  - `virtual ~User()` - 虚析构函数

#### 1.2 派生类：`Student`
- **属性**：
  - `string studentId` - 学号
  - `string className` - 班级
  - `int maxQuota` - 最大资源配额
- **方法**：
  - `int calculateQuota() override` - 实现学生配额计算

#### 1.3 派生类：`Teacher`
- **属性**：
  - `string employeeId` - 工号
  - `string department` - 部门
  - `int maxQuota` - 最大资源配额
- **方法**：
  - `int calculateQuota() override` - 实现教师配额计算（高于学生）

#### 1.4 派生类：`Administrator`
- **属性**：
  - `int permissionLevel` - 权限等级
- **方法**：
  - `int calculateQuota() override` - 实现管理员配额计算（最高）

### 2. 资源相关类

#### 2.1 基类：`CloudResource`（抽象基类）
- **属性**：
  - `string resourceId` - 资源ID
  - `string name` - 资源名称
  - `string type` - 资源类型
  - `double basePrice` - 基础价格
  - `ResourceStatus status` - 资源状态（枚举类型）
- **方法**：
  - `double getUtilizationRate()` - 获取资源利用率
  - `string getResourceInfo()` - 获取资源详细信息
  - `virtual double calculatePrice(int hours) = 0` - 计算租用价格（纯虚函数）

#### 2.2 派生类：`CPUResource`
- **属性**：
  - `int cores` - CPU核心数
  - `double frequency` - CPU频率
- **方法**：
  - `double calculatePrice(int hours) override` - 根据核心数和时间计算价格

#### 2.3 派生类：`GPUResource`
- **属性**：
  - `string model` - GPU型号
  - `int memory` - 显存大小
- **方法**：
  - `double calculatePrice(int hours) override` - 根据型号和时间计算价格

#### 2.4 派生类：`StorageResource`
- **属性**：
  - `int capacity` - 存储容量
  - `double readWriteSpeed` - 读写速度
- **方法**：
  - `double calculatePrice(int hours) override` - 根据容量和时间计算价格

### 3. 租用相关类

#### 3.1 `Rental`类
- **属性**：
  - `string rentalId` - 租用ID
  - `string userId` - 用户ID
  - `string resourceId` - 资源ID
  - `time_t startTime` - 开始时间
  - `time_t endTime` - 结束时间
  - `double fee` - 费用
  - `RentalStatus status` - 租用状态（枚举类型）
  - `string reviewerId` - 审核人ID
  - `time_t reviewTime` - 审核时间
  - `string reviewComment` - 审核备注
- **方法**：
  - `void updateStatus(RentalStatus newStatus)` - 更新租用状态
  - `int calculateDuration()` - 计算租用时长（小时）
  - `string getRentalInfo()` - 获取租用详情

### 4. 支付与计费类

#### 4.1 `BillingService`类
- **属性**：
  - `map<string, double> pricingRules` - 计费规则表
  - `int minBillingUnit` - 最小计费单位（小时）
  - `int maxRentalTime` - 最长租用时间（小时）
- **方法**：
  - `double calculateFee(Rental& rental, CloudResource& resource)` - 计算费用
  - `bool updateUserBalance(User* user, double amount)` - 更新用户余额
  - `string generateBill(Rental& rental)` - 生成账单
  - `void handleInsufficientBalance(User* user, double requiredAmount)` - 处理余额不足

### 5. 通知服务类

#### 5.1 `Notification`类
- **属性**：
  - `string notificationId` - 通知ID
  - `string userId` - 用户ID
  - `string content` - 通知内容
  - `time_t createTime` - 创建时间
  - `int priority` - 优先级
  - `bool isRead` - 是否已读
- **方法**：
  - `void markAsRead()` - 标记为已读
  - `string getNotificationInfo()` - 获取通知详情

#### 5.2 `NotificationService`类
- **属性**：
  - `vector<Notification> notifications` - 通知列表
- **方法**：
  - `void checkRentalTimeout()` - 检查租用超时
  - `void checkInsufficientBalance()` - 检查余额不足
  - `void addNotification(string userId, string content, int priority)` - 添加通知
  - `vector<Notification> getNotifications(string userId, bool unreadOnly)` - 获取用户通知
  - `void clearNotifications(string userId)` - 清除用户通知

### 6. 系统管理类

#### 6.1 `UserManager`类
- **属性**：
  - `vector<User*> users` - 用户列表
- **方法**：
  - `User* login(string id, string password)` - 用户登录
  - `bool registerUser(User* user)` - 用户注册
  - `User* getUserById(string id)` - 根据ID获取用户
  - `vector<User*> getAllUsers()` - 获取所有用户
  - `bool updateUser(User* user)` - 更新用户信息
  - `void saveUsers()` - 保存用户数据
  - `void loadUsers()` - 加载用户数据

#### 6.2 `ResourceManager`类
- **属性**：
  - `vector<CloudResource*> resources` - 资源列表
- **方法**：
  - `bool addResource(CloudResource* resource)` - 添加资源
  - `bool updateResource(CloudResource* resource)` - 更新资源
  - `bool deleteResource(string resourceId)` - 删除资源
  - `CloudResource* getResourceById(string id)` - 根据ID获取资源
  - `vector<CloudResource*> getResourcesByType(string type)` - 根据类型获取资源
  - `vector<CloudResource*> getAllResources()` - 获取所有资源
  - `void saveResources()` - 保存资源数据
  - `void loadResources()` - 加载资源数据

#### 6.3 `RentalManager`类
- **属性**：
  - `vector<Rental> rentals` - 租用记录列表
- **方法**：
  - `bool createRental(string userId, string resourceId, int hours)` - 创建租用申请
  - `bool reviewRental(string rentalId, string reviewerId, bool approved, string comment)` - 审核租用申请
  - `bool cancelRental(string rentalId)` - 取消租用
  - `bool completeRental(string rentalId)` - 完成租用
  - `vector<Rental> getRentalsByUserId(string userId)` - 获取用户租用记录
  - `vector<Rental> getAllRentals()` - 获取所有租用记录
  - `Rental* getRentalById(string rentalId)` - 根据ID获取租用记录
  - `void saveRentals()` - 保存租用数据
  - `void loadRentals()` - 加载租用数据

#### 6.4 `LogManager`类
- **属性**：
  - `vector<string> logs` - 日志列表
- **方法**：
  - `void addLog(string operation, string userId, string details)` - 添加日志
  - `vector<string> getLogs()` - 获取所有日志
  - `void saveLogs()` - 保存日志数据
  - `void loadLogs()` - 加载日志数据

#### 6.5 `SystemManager`类（系统总控类）
- **属性**：
  - `UserManager userManager` - 用户管理器
  - `ResourceManager resourceManager` - 资源管理器
  - `RentalManager rentalManager` - 租用管理器
  - `BillingService billingService` - 计费服务
  - `NotificationService notificationService` - 通知服务
  - `LogManager logManager` - 日志管理器
- **方法**：
  - `void initialize()` - 初始化系统
  - `void run()` - 运行系统
  - `void shutdown()` - 关闭系统
  - `void saveAllData()` - 保存所有数据
  - `void loadAllData()` - 加载所有数据

### 7. 异常处理类

#### 7.1 基类：`Exception`
- **属性**：
  - `int errorCode` - 错误代码
  - `string errorMessage` - 错误信息
- **方法**：
  - `string getErrorMessage()` - 获取错误信息
  - `int getErrorCode()` - 获取错误代码

#### 7.2 派生类：`ResourceException`
- 处理资源相关异常（资源不足、资源状态异常等）

#### 7.3 派生类：`UserException`
- 处理用户相关异常（余额不足、权限不足等）

#### 7.4 派生类：`SystemException`
- 处理系统相关异常（文件操作失败、数据损坏等）

### 8. 辅助类

#### 8.1 `Utils`类（工具类）
- **方法**：
  - `string generateUniqueId()` - 生成唯一ID
  - `string encryptPassword(string password)` - 密码加密
  - `string formatDateTime(time_t time)` - 格式化日期时间
  - `int calculateHours(time_t start, time_t end)` - 计算小时数

#### 8.2 `Menu`类（菜单类）
- **方法**：
  - `void showMainMenu()` - 显示主菜单
  - `void showUserMenu(User* user)` - 显示用户菜单
  - `void showAdminMenu()` - 显示管理员菜单
  - `void showResourceMenu()` - 显示资源菜单
  - `void showRentalMenu()` - 显示租用菜单

## 二、枚举类型定义

### 1. 用户状态枚举
```cpp
enum class UserStatus {
    ACTIVE,     // 活跃
    SUSPENDED,  // 暂停
    DELETED     // 已删除
};
```

### 2. 资源状态枚举
```cpp
enum class ResourceStatus {
    AVAILABLE,  // 可用
    OCCUPIED,   // 已占用
    MAINTENANCE,// 维护中
    DELETED     // 已删除
};
```

### 3. 租用状态枚举
```cpp
enum class RentalStatus {
    PENDING,    // 申请中
    APPROVED,   // 已批准
    REJECTED,   // 已拒绝
    USING,      // 使用中
    COMPLETED,  // 已完成
    CANCELLED   // 已取消
};
```

## 三、数据文件结构

### 1. `users.dat`
- 存储所有用户信息，包括基本信息和特定角色信息
- 二进制格式，支持不同类型用户的序列化和反序列化

### 2. `resources.dat`
- 存储所有云算力资源信息，包括基本信息和特定类型资源信息
- 二进制格式，支持不同类型资源的序列化和反序列化

### 3. `rentals.dat`
- 存储所有租用记录，包括申请信息、审核信息、使用信息等
- 二进制格式，支持租用记录的序列化和反序列化

### 4. `logs.dat`
- 存储系统操作日志，包括操作时间、操作用户、操作内容等
- 二进制格式，支持日志的序列化和反序列化

### 5. `notifications.dat`
- 存储系统通知信息，包括通知内容、优先级、创建时间等
- 二进制格式，支持通知的序列化和反序列化

### 6. `billing_rules.dat`
- 存储计费规则，包括不同资源类型的基础价格、计费系数等
- 二进制格式，支持计费规则的序列化和反序列化

## 四、功能实现流程

### 1. 系统启动流程
1. 初始化各管理器和服务
2. 加载所有数据文件
3. 显示登录/注册界面
4. 等待用户操作

### 2. 用户注册流程
1. 用户选择角色（学生/教师）
2. 输入基本信息和角色特定信息
3. 系统验证信息有效性
4. 创建用户对象并保存
5. 返回登录界面

### 3. 用户登录流程
1. 用户输入ID和密码
2. 系统验证身份
3. 根据用户角色显示相应菜单
4. 检查并显示未读通知

### 4. 资源租用流程
1. 用户浏览可用资源
2. 选择资源并指定租用时长
3. 系统检查用户配额和资源状态
4. 创建租用申请，状态为"申请中"
5. 等待管理员审核

### 5. 租用审核流程
1. 管理员查看待审核申请
2. 审核申请（通过/拒绝）并填写备注
3. 系统更新租用状态
4. 如果通过，分配资源给用户；如果拒绝，通知用户

### 6. 租用完成流程
1. 用户归还资源或租用时间到期
2. 系统计算使用费用
3. 从用户余额扣除费用
4. 生成账单
5. 更新资源状态为可用

### 7. 异常处理流程
1. 余额不足：提示充值，暂停申请处理
2. 资源不足：进入等待队列或推荐替代资源
3. 租用超时：自动释放资源，计算费用，可能限制用户后续使用
4. 系统故障：记录日志，尝试恢复，必要时通知管理员

## 五、系统安全与数据保护

### 1. 密码安全
- 密码加密存储，不明文保存
- 密码输入不显示，防止泄露

### 2. 操作安全
- 关键操作记录日志，包括操作时间、操作用户、操作内容
- 敏感操作需要二次确认

### 3. 权限控制
- 严格区分不同角色的操作权限
- 防止越权操作

### 4. 数据安全
- 定期备份数据文件
- 数据文件加密存储
- 异常退出时自动保存数据

## 六、系统测试计划

### 1. 单元测试
- 测试各个类和方法的功能
- 验证数据结构的正确性

### 2. 集成测试
- 测试模块间的交互
- 验证数据流的正确性

### 3. 系统测试
- 测试整个系统的功能和性能
- 验证异常处理机制

### 4. 用户测试
- 邀请实际用户测试系统
- 收集用户反馈并优化系统



        