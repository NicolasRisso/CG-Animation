#pragma once
#include <unordered_map>
#include <cvt/wstring>
#include <any>

class SceneObject;

class IComponent
{
public:
    virtual ~IComponent() = default;

    virtual void Start() = 0;
    virtual void Tick(const float DeltaTime) = 0;

    void SetOwner(SceneObject* owner) { m_Owner = owner; }
    SceneObject* GetOwner() const { return m_Owner; }

    // Memory Logic
    template<typename T>
    void SetVariable(const std::string& name, const T&& value)
    {
        m_MemoryVariables[name] = std::forward<T>(value);
    }
    template<typename T>
    T GetVariable(const std::string& name) const
    {
        const auto it = m_MemoryVariables.find(name);
        if (it == m_MemoryVariables.end())
            throw std::runtime_error("No such variable: " + name);
        return std::any_cast<T>(it->second);
    }

    bool HasVariable(const std::string& name) const
    {
        return m_MemoryVariables.find(name) != m_MemoryVariables.end();
    }

    void RemoveVariable(const std::string& name)
    {
        m_MemoryVariables.erase(name);
    }

private:
    SceneObject* m_Owner = nullptr;

    std::unordered_map<std::string, std::any> m_MemoryVariables;
};
