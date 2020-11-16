#include "pch.hh"

#include "System.hh"

using namespace IceSDK::Core::ECS;

System::System(const std::weak_ptr<entt::registry>& wpRegistry)
{
    this->m_wpRegistry = wpRegistry;
}
