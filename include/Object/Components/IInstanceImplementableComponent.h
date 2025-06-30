#pragma once

#include "IComponent.h"
#include <functional>
#include <utility>

class IInstanceImplementableComponent : public IComponent
{
public:
    using StartFunction = std::function<void()>;
    using TickFunction = std::function<void(float)>;

    explicit IInstanceImplementableComponent(StartFunction startFunction = nullptr, TickFunction tickFunction = nullptr)
        : m_StartFunction(std::move(startFunction)), m_TickFunction(std::move(tickFunction)) {}

    void SetStartFunction(StartFunction startFunction) { m_StartFunction = std::move(startFunction); }
    void SetTickFunction(TickFunction tickFunction) { m_TickFunction = std::move(tickFunction); }

    void Start() override { if (m_StartFunction) m_StartFunction(); }
    void Tick(const float DeltaTime) override { if (m_TickFunction) m_TickFunction(DeltaTime); }

private:
    StartFunction m_StartFunction;
    TickFunction m_TickFunction;
};