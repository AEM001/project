// Resource.hpp
#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <map> // 用于资源参数
#include <memory> // 智能指针
#include <fstream> // 用于文件操作

// 资源类型枚举
enum class ResourceType {
    CPU,
    GPU,
    // 根据需要添加其他类型
};

// 资源状态枚举
enum class ResourceStatus {
    IDLE,
    IN_USE,
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
    double Storage;
    double hourprice;
    
public:
    // 构造函数 - 修改参数名保持一致性
    Resource(std::string id, std::string name, ResourceType t, double rate, 
             ResourceStatus stat = ResourceStatus::IDLE, double storage = 50)
        : resourceId(std::move(id)),
          resourceName(std::move(name)),
          type(t),
          hourprice(rate),
          status(stat),
          Storage(storage){} 
    virtual ~Resource() = default;

    // 获取器
    std::string getResourceId() const{return resourceId;}
    std::string getResourceName() const{return resourceName;}
    ResourceType getResourceType() const{return type;}
    ResourceStatus getStatus() const{return status;}
    // std::string getParameter(const std::string& paramName) const{return parameters.at(paramName);} // 获取特定参数
    // const std::map<std::string, std::string>& getAllParameters() const{return parameters;}
    double getHourlyRate() const{return hourprice;}

    // 设置器
    void setResourceName(const std::string& newName){resourceName=newName;}
    void setStatus(ResourceStatus newStatus){status=newStatus;}
    // void setParameter(const std::string& paramName, const std::string& value){parameters[paramName]=value;} // 设置或更新参数
    void setHourlyRate(double newRate){hourprice=newRate;}

    // 显示资源特定详情的虚函数
    virtual void displayDetails() const = 0;

    // 检查资源是否可供租用
    bool isAvailable() const{return status==ResourceStatus::IDLE;}

    // 序列化/反序列化方法（用于数据持久化）
    virtual void serialize(std::ostream& os) const {
        // 写入基本属性
        os.write(resourceId.c_str(), resourceId.size() + 1);
        os.write(resourceName.c_str(), resourceName.size() + 1);
        os.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));
        os.write(reinterpret_cast<const char*>(&status), sizeof(ResourceStatus));
        os.write(reinterpret_cast<const char*>(&Storage), sizeof(double));
        os.write(reinterpret_cast<const char*>(&hourprice), sizeof(double));
    }
    
    virtual void deserialize(std::istream& is) {
        // 读取基本属性
        char buffer[256];
        is.getline(buffer, 256, '\0');
        resourceId = buffer;
        
        is.getline(buffer, 256, '\0');
        resourceName = buffer;
        
        is.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));
        is.read(reinterpret_cast<char*>(&status), sizeof(ResourceStatus));
        is.read(reinterpret_cast<char*>(&Storage), sizeof(double));
        is.read(reinterpret_cast<char*>(&hourprice), sizeof(double));
    }
};

/**
 * @class CPUResource
 * @brief 表示CPU资源。
 *
 * 继承自Resource。具有特定参数，如核心数量、频率。
 */
class CPUResource : public Resource {
private:
    int coreCount;
    double frequency; // 单位：GHz
public:
    CPUResource(std::string id, std::string name, double rate ,int cores, double frequency,ResourceType t=ResourceType::CPU , ResourceStatus stat = ResourceStatus::IDLE,double Storage=50):
        Resource(id,name,t,rate,stat,Storage),coreCount(cores),frequency(frequency){}
    void displayDetails() const override
    {
        std::cout << "CPU Resource: " << resourceName << " (ID: " << resourceId << ")\n";
        std::cout << "Type: " << (type == ResourceType::CPU ? "CPU" : "GPU") << "\n";
        std::cout << "Status: " << (status == ResourceStatus::IDLE? "Available" : "In Use") << "\n";
        std::cout << "Core Count: " << coreCount << "\n";
        std::cout << "Frequency: " << frequency << " GHz\n";
        std::cout << "Hourly Rate: $" << hourprice << "/hour\n";
        std::cout << "Storage: " << Storage << " GB\n";

    }
    // CPU特定方法或参数访问器
    int getCoreCount() const{return coreCount;}
    double getFrequency() const{return frequency;}
    void serialize(std::ostream& os) const override {
        // 先序列化基类部分
        Resource::serialize(os);
        // 再序列化子类特有属性
        os.write(reinterpret_cast<const char*>(&coreCount), sizeof(int));
        os.write(reinterpret_cast<const char*>(&frequency), sizeof(double));
    }
    
    void deserialize(std::istream& is) override {
        // 先反序列化基类部分
        Resource::deserialize(is);
        // 再反序列化子类特有属性
        is.read(reinterpret_cast<char*>(&coreCount), sizeof(int));
        is.read(reinterpret_cast<char*>(&frequency), sizeof(double));
    }
};

/**
 * @class GPUResource
 * @brief 表示GPU资源。
 *
 * 继承自Resource。可能具有特定参数，如显存、型号。
 */
class GPUResource : public Resource {
private:
    int cudaCores;
    int vramG; // 显存容量，单位：G
public:
    GPUResource(std::string id, std::string name, double rate ,int cudacores,int vram,ResourceType t=ResourceType::GPU , ResourceStatus stat = ResourceStatus::IDLE,double Storage=50):
    Resource(id,name,t,rate,stat,Storage),cudaCores(cudacores),vramG(vram){}
    void displayDetails() const override
    {
        std::cout << "GPU Resource: " << resourceName << " (ID: " << resourceId << ")\n";
        std::cout << "Type: " << (type == ResourceType::CPU? "CPU" : "GPU") << "\n";
        std::cout << "Status: " << (status == ResourceStatus::IDLE? "Available" : "In Use") << "\n";
        std::cout << "Cuda Core Count: " << cudaCores << "\n";
        std::cout << "VRAM: " << vramG << " GB\n";
        std::cout << "Hourly Rate: $" << hourprice << "/hour\n";
        std::cout << "Storage: " << Storage << " GB\n";

    }
    // GPU特定方法and参数访问器
    int getCudaCores() const{return cudaCores;}
    int getVRAM() const{return vramG;} // 单位：G
    void serialize(std::ostream& os) const override {
        // 先序列化基类部分
        Resource::serialize(os);
        // 再序列化子类特有属性
        os.write(reinterpret_cast<const char*>(&cudaCores), sizeof(int));
        os.write(reinterpret_cast<const char*>(&vramG), sizeof(int));
    }
    
    void deserialize(std::istream& is) override {
        // 先反序列化基类部分
        Resource::deserialize(is);
        // 再反序列化子类特有属性
        is.read(reinterpret_cast<char*>(&cudaCores), sizeof(int));
        is.read(reinterpret_cast<char*>(&vramG), sizeof(int));
    }
};


/**
 * @class ResourceCollection
 * @brief 管理系统中所有计算资源的集合。
 *
 * 提供添加、查找、列出资源的功能。
 * 可以按类型筛选资源。
 */
class ResourceCollection {
private:
    std::vector<std::shared_ptr<Resource>> resources;

public:
    // 添加资源到集合
    void addResource(std::shared_ptr<Resource> resource) {
        resources.push_back(resource);
    }

    // 根据ID查找资源
    std::shared_ptr<Resource> findResourceById(const std::string& id) const {
        for (const auto& resource : resources) {
            if (resource->getResourceId() == id) {
                return resource;
            }
        }
        return nullptr; // 未找到资源
    }

    // 获取所有资源
    const std::vector<std::shared_ptr<Resource>>& getAllResources() const {
        return resources;
    }

    // 获取特定类型的资源
    std::vector<std::shared_ptr<Resource>> getResourcesByType(ResourceType type) const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == type) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 获取所有可用资源
    std::vector<std::shared_ptr<Resource>> getAvailableResources() const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->isAvailable()) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 获取特定类型的可用资源
    std::vector<std::shared_ptr<Resource>> getAvailableResourcesByType(ResourceType type) const {
        std::vector<std::shared_ptr<Resource>> result;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == type && resource->isAvailable()) {
                result.push_back(resource);
            }
        }
        return result;
    }

    // 显示所有资源
    void displayAllResources() const {
        std::cout << "===== 所有资源列表 =====\n";
        for (const auto& resource : resources) {
            resource->displayDetails();
            std::cout << "------------------------\n";
        }
    }

    // 显示特定类型的资源
    void displayResourcesByType(ResourceType type) const {
        std::cout << "===== " << (type == ResourceType::CPU ? "CPU" : "GPU") << " 资源列表 =====\n";
        for (const auto& resource : resources) {
            if (resource->getResourceType() == type) {
                resource->displayDetails();
                std::cout << "------------------------\n";
            }
        }
    }
    
    // 返回可用CPU资源型号列表
    std::vector<std::string> getAvailableCPUModels() const {
        std::vector<std::string> models;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == ResourceType::CPU && resource->isAvailable()) {
                models.push_back(resource->getResourceName());
            }
        }
        return models;
    }
    
    // 返回可用GPU资源型号列表
    std::vector<std::string> getAvailableGPUModels() const {
        std::vector<std::string> models;
        for (const auto& resource : resources) {
            if (resource->getResourceType() == ResourceType::GPU && resource->isAvailable()) {
                models.push_back(resource->getResourceName());
            }
        }
        return models;
    }
    
    // 打印可用CPU资源型号
    void displayAvailableCPUModels() const {
        std::cout << "===== 可用CPU型号列表 =====\n";
        auto models = getAvailableCPUModels();
        if (models.empty()) {
            std::cout << "当前没有可用的CPU资源\n";
        } else {
            for (const auto& model : models) {
                std::cout << "- " << model << "\n";
            }
        }
    }
    
    // 打印可用GPU资源型号
    void displayAvailableGPUModels() const {
        std::cout << "===== 可用GPU型号列表 =====\n";
        auto models = getAvailableGPUModels();
        if (models.empty()) {
            std::cout << "当前没有可用的GPU资源\n";
        } else {
            for (const auto& model : models) {
                std::cout << "- " << model << "\n";
            }
        }
    }

    // 持久化方法
    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行写入: " + filename);
        }
        
        // 写入资源数量
        size_t count = resources.size();
        file.write(reinterpret_cast<const char*>(&count), sizeof(size_t));
        
        // 逐个写入资源
        for (const auto& resource : resources) {
            // 写入资源类型标识
            ResourceType type = resource->getResourceType();
            file.write(reinterpret_cast<const char*>(&type), sizeof(ResourceType));
            
            // 根据类型序列化资源
            resource->serialize(file);
        }
        
        file.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("无法打开文件进行读取: " + filename);
        }
        
        // 清空当前资源
        resources.clear();
        
        // 读取资源数量
        size_t count;
        file.read(reinterpret_cast<char*>(&count), sizeof(size_t));
        
        // 逐个读取资源
        for (size_t i = 0; i < count; ++i) {
            // 读取资源类型标识
            ResourceType type;
            file.read(reinterpret_cast<char*>(&type), sizeof(ResourceType));
            
            // 根据类型创建相应的资源对象
            std::shared_ptr<Resource> resource;
            if (type == ResourceType::CPU) {
                resource = std::make_shared<CPUResource>("", "", 0, 0, 0);
            } else if (type == ResourceType::GPU) {
                resource = std::make_shared<GPUResource>("", "", 0, 0, 0);
            } else {
                throw std::runtime_error("未知的资源类型");
            }
            
            // 反序列化资源
            resource->deserialize(file);
            
            // 添加到集合
            resources.push_back(resource);
        }
        
        file.close();
    }
};

// 创建预设资源集合的辅助函数
inline ResourceCollection createDefaultResourceCollection() {
    ResourceCollection collection;
    
    // 添加CPU资源 - 高性能服务器计算型号
    // 精简后的CPU资源配置，主要型号多台，总数20台
    collection.addResource(std::make_shared<CPUResource>("CPU001", "Intel Xeon Platinum 8380", 4.0, 40, 2.3, ResourceType::CPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU002", "Intel Xeon Platinum 8380", 4.0, 40, 2.3, ResourceType::CPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU003", "Intel Xeon Gold 6348", 3.5, 28, 2.6, ResourceType::CPU, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU004", "Intel Xeon Gold 6348", 3.5, 28, 2.6, ResourceType::CPU, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU005", "AMD EPYC 7763", 4.2, 64, 2.45, ResourceType::CPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU006", "AMD EPYC 7763", 4.2, 64, 2.45, ResourceType::CPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<CPUResource>("CPU007", "Intel Xeon Platinum 8480+", 4.5, 56, 2.0, ResourceType::CPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<CPUResource>("CPU008", "Intel Xeon Platinum 8480+", 4.5, 56, 2.0, ResourceType::CPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<CPUResource>("CPU009", "AMD EPYC 7713", 3.8, 64, 2.0, ResourceType::CPU, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU010", "AMD EPYC 7713", 3.8, 64, 2.0, ResourceType::CPU, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU011", "Intel Xeon Silver 4314", 2.5, 16, 2.4, ResourceType::CPU, ResourceStatus::IDLE, 128));
    collection.addResource(std::make_shared<CPUResource>("CPU012", "Intel Xeon Silver 4314", 2.5, 16, 2.4, ResourceType::CPU, ResourceStatus::IDLE, 128));
    collection.addResource(std::make_shared<CPUResource>("CPU013", "AMD EPYC 7543", 3.0, 32, 2.8, ResourceType::CPU, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU014", "AMD EPYC 7543", 3.0, 32, 2.8, ResourceType::CPU, ResourceStatus::IDLE, 256));
    collection.addResource(std::make_shared<CPUResource>("CPU015", "Intel Core i9-13900K", 3.0, 24, 3.0, ResourceType::CPU, ResourceStatus::IDLE, 64));
    collection.addResource(std::make_shared<CPUResource>("CPU016", "Intel Core i9-13900K", 3.0, 24, 3.0, ResourceType::CPU, ResourceStatus::IDLE, 64));
    collection.addResource(std::make_shared<CPUResource>("CPU017", "AMD Ryzen 9 7950X", 3.5, 16, 4.5, ResourceType::CPU, ResourceStatus::IDLE, 64));
    collection.addResource(std::make_shared<CPUResource>("CPU018", "AMD Ryzen 9 7950X", 3.5, 16, 4.5, ResourceType::CPU, ResourceStatus::IDLE, 64));
    collection.addResource(std::make_shared<CPUResource>("CPU019", "Intel Xeon E5-2699 v4", 2.0, 22, 2.2, ResourceType::CPU, ResourceStatus::IDLE, 128));
    collection.addResource(std::make_shared<CPUResource>("CPU020", "Intel Xeon E5-2699 v4", 2.0, 22, 2.2, ResourceType::CPU, ResourceStatus::IDLE, 128));

    // 添加GPU资源 - 全部为NVIDIA高性能计算型号及部分AMD型号
    collection.addResource(std::make_shared<GPUResource>("GPU001", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceType::GPU, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU002", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceType::GPU, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU003", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceType::GPU, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU004", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceType::GPU, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU005", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceType::GPU, ResourceStatus::IDLE, 4096));
    collection.addResource(std::make_shared<GPUResource>("GPU006", "NVIDIA H100 80GB", 10.0, 16896, 80, ResourceType::GPU, ResourceStatus::IDLE, 4096));

    collection.addResource(std::make_shared<GPUResource>("GPU007", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceType::GPU, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU008", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceType::GPU, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU009", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceType::GPU, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU010", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceType::GPU, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU011", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceType::GPU, ResourceStatus::IDLE, 2048));
    collection.addResource(std::make_shared<GPUResource>("GPU012", "NVIDIA A100 80GB", 8.0, 6912, 80, ResourceType::GPU, ResourceStatus::IDLE, 2048));

    collection.addResource(std::make_shared<GPUResource>("GPU013", "NVIDIA A100 40GB", 7.0, 6912, 40, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU014", "NVIDIA A100 40GB", 7.0, 6912, 40, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU015", "NVIDIA A100 40GB", 7.0, 6912, 40, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU016", "NVIDIA A100 40GB", 7.0, 6912, 40, ResourceType::GPU, ResourceStatus::IDLE, 1024));

    collection.addResource(std::make_shared<GPUResource>("GPU017", "NVIDIA Tesla V100", 7.0, 5120, 32, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU018", "NVIDIA Tesla V100", 7.0, 5120, 32, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU019", "NVIDIA Tesla V100", 7.0, 5120, 32, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU020", "NVIDIA Tesla V100", 7.0, 5120, 32, ResourceType::GPU, ResourceStatus::IDLE, 512));

    collection.addResource(std::make_shared<GPUResource>("GPU021", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU022", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU023", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU024", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceType::GPU, ResourceStatus::IDLE, 512));
    collection.addResource(std::make_shared<GPUResource>("GPU025", "NVIDIA RTX 4090", 6.5, 16384, 24, ResourceType::GPU, ResourceStatus::IDLE, 512));

    collection.addResource(std::make_shared<GPUResource>("GPU026", "NVIDIA RTX 5090", 7.0, 20480, 32, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU027", "NVIDIA RTX 5090", 7.0, 20480, 32, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU028", "NVIDIA RTX 5090", 7.0, 20480, 32, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU029", "NVIDIA RTX 5090", 7.0, 20480, 32, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    collection.addResource(std::make_shared<GPUResource>("GPU030", "NVIDIA RTX 5090", 7.0, 20480, 32, ResourceType::GPU, ResourceStatus::IDLE, 1024));
    
    return collection;
}

#endif // RESOURCE_HPP
