// Resource.hpp
#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <vector>
#include <map> // 用于资源参数

// 资源类型枚举
enum class ResourceType {
    CPU,
    GPU,
    STORAGE
    // 根据需要添加其他类型
};

// 资源状态枚举
enum class ResourceStatus {
    IDLE,
    IN_USE,
    MAINTENANCE // 可选：用于临时不可用的资源
};

/**
 * @class Resource
 * @brief 系统中所有计算资源的基类。
 *
 * 存储通用资源信息，如ID、类型、名称、状态和具体参数。
 * 提供检查可用性和管理状态的功能。
 * 交互对象：
 *  - 租赁模块：作为租赁记录的一部分并用于状态更新。
 *  - 管理员模块：用于管理（添加、修改、删除）。
 */
class Resource {
protected:
    std::string resourceId;
    std::string resourceName;
    ResourceType type;
    ResourceStatus status;
    std::map<std::string, std::string> parameters; // 例如：{"cores": "4", "frequency": "3.0GHz"} 或 {"capacity": "1TB"}
    double hourlyRate; // 此资源的每小时成本

public:
    // 构造函数
    Resource(std::string id, std::string name, ResourceType t, double rate, ResourceStatus stat = ResourceStatus::IDLE);
    virtual ~Resource() = default;

    // 获取器
    std::string getResourceId() const;
    std::string getResourceName() const;
    ResourceType getResourceType() const;
    ResourceStatus getStatus() const;
    std::string getParameter(const std::string& paramName) const; // 获取特定参数
    const std::map<std::string, std::string>& getAllParameters() const;
    double getHourlyRate() const;

    // 设置器
    void setResourceName(const std::string& newName);
    void setStatus(ResourceStatus newStatus);
    void setParameter(const std::string& paramName, const std::string& value); // 设置或更新参数
    void setHourlyRate(double newRate);

    // 显示资源特定详情的虚函数
    virtual void displayDetails() const = 0;

    // 检查资源是否可供租用
    bool isAvailable() const;

    // 序列化/反序列化方法（用于数据持久化）
    // virtual void serialize(std::ostream& os) const;
    // virtual void deserialize(std::istream& is);
};

/**
 * @class CPUResource
 * @brief 表示CPU资源。
 *
 * 继承自Resource。可能具有特定参数，如核心数量、频率。
 */
class CPUResource : public Resource {
public:
    CPUResource(std::string id, std::string name, double rate, int cores, double frequency);
    void displayDetails() const override;
    // CPU特定方法或参数访问器
    int getCoreCount() const;
    double getFrequency() const;
};

/**
 * @class GPUResource
 * @brief 表示GPU资源。
 *
 * 继承自Resource。可能具有特定参数，如显存、型号。
 */
class GPUResource : public Resource {
public:
    GPUResource(std::string id, std::string name, double rate, std::string model, int vramMB);
    void displayDetails() const override;
    // GPU特定方法或参数访问器
    std::string getModel() const;
    int getVRAM() const; // 单位：MB
};

/**
 * @class StorageResource
 * @brief 表示存储资源。
 *
 * 继承自Resource。可能具有特定参数，如容量、类型（SSD/HDD）。
 */
class StorageResource : public Resource {
public:
    StorageResource(std::string id, std::string name, double rate, int capacityGB, std::string storageType);
    void displayDetails() const override;
    // 存储特定方法或参数访问器
    int getCapacity() const; // 单位：GB
    std::string getStorageType() const;
};

#endif // RESOURCE_HPP