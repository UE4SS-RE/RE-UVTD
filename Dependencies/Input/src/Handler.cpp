#include <array>

#include <Input/Handler.hpp>

#define NOMINMAX
#include <Windows.h>

namespace RC::Input
{
    auto is_modifier_key_required(ModifierKey modifier_key, std::vector<ModifierKey> modifier_keys) -> bool
    {
        for (const auto& required_modifier_key : modifier_keys)
        {
            if (required_modifier_key == ModifierKey::MOD_KEY_START_OF_ENUM)
            {
                continue;
            }

            if (modifier_key == required_modifier_key)
            {
                return true;
            }
        }

        return false;
    }

    auto Handler::are_modifier_keys_down(const std::vector<ModifierKey>& required_modifier_keys) -> bool
    {
        bool are_required_modifier_keys_down = true;

        for (const auto& [modifier_key, modifier_key_is_down] : m_modifier_keys_down)
        {
            for (const auto& required_modifier_key : required_modifier_keys)
            {
                if (modifier_key == required_modifier_key && !modifier_key_is_down)
                {
                    are_required_modifier_keys_down = false;
                }

                if (modifier_key != required_modifier_key && modifier_key_is_down && !is_modifier_key_required(modifier_key, required_modifier_keys))
                {
                    return false;
                }
            }
        }

        return are_required_modifier_keys_down;
    }

    auto Handler::is_program_focused() -> bool
    {
        HWND hwnd = GetForegroundWindow();
        if (!hwnd) return false;

        wchar_t current_window_class_name[MAX_PATH];
        if (!GetClassNameW(hwnd, current_window_class_name, MAX_PATH)) return false;

        for (const auto& active_window_class : m_active_window_classes)
        {
            if (wcscmp(current_window_class_name, active_window_class) == 0)
            {
                return true;
            }
        }

        return false;
    }

    auto Handler::process_event() -> void
    {
        if (!is_program_focused())
        {
            return;
        }

        std::vector<EventCallbackCallable> callbacks_to_call{};

        bool skip_this_frame = !get_allow_input();
        bool is_any_modifier_keys_down = false;
        bool any_keys_are_down = false;

        if (m_any_keys_are_down)
        {
            skip_this_frame = true;
        }

        // Check if any modifier keys are down
        for (auto& [modifier_key, key_is_down] : m_modifier_keys_down)
        {
            if (GetAsyncKeyState(modifier_key))
            {
                is_any_modifier_keys_down = true;
                key_is_down = true;
            }
            else
            {
                key_is_down = false;
            }
        }

        for (auto& key_set_data : m_key_sets)
        {
            for (auto& [key, key_data_array] : key_set_data.key_data)
            {
                for (auto& key_data : key_data_array)
                {
                    if (GetAsyncKeyState(key) && !key_data.is_down)
                    {
                        any_keys_are_down = true;
                        bool should_propagate = true;

                        if (key_data.requires_modifier_keys)
                        {
                            if (!are_modifier_keys_down(key_data.required_modifier_keys))
                            {
                                should_propagate = false;
                            }
                        }

                        if (!key_data.requires_modifier_keys && is_any_modifier_keys_down)
                        {
                            should_propagate = false;
                        }

                        if (should_propagate)
                        {
                            key_data.is_down = true;
                            for (const auto& callback : key_data.callbacks)
                            {
                                callbacks_to_call.emplace_back(callback);
                            }
                        }
                    }
                    else if (!GetAsyncKeyState(key) && key_data.is_down)
                    {
                        key_data.is_down = false;
                    }
                }
            }
        }

        if (any_keys_are_down)
        {
            m_any_keys_are_down = true;
        }
        else
        {
            m_any_keys_are_down = false;
        }

        for (const auto& callback : callbacks_to_call)
        {
            if (skip_this_frame)
            {
                return;
            }
            callback();
        }
    }

    auto Handler::register_keydown_event(Input::Key key, EventCallbackCallable callback, uint8_t custom_data) -> void
    {
        KeySet& key_set = [&]() -> KeySet& {
            for (auto& key_set : m_key_sets)
            {
                if (key_set.key_data.contains(key))
                {
                    return key_set;
                }
            }

            return m_key_sets.emplace_back(KeySet{});
        }();

        KeyData& key_data = key_set.key_data[key].emplace_back();
        key_data.callbacks.emplace_back(callback);
        key_data.custom_data = custom_data;
    }

    auto Handler::register_keydown_event(Input::Key key, const ModifierKeyArray& modifier_keys, const EventCallbackCallable& callback, uint8_t custom_data) -> void
    {
        KeySet& key_set = [&]() -> KeySet& {
            for (auto& key_set : m_key_sets)
            {
                if (key_set.key_data.contains(key))
                {
                    return key_set;
                }
            }

            return m_key_sets.emplace_back(KeySet{});
        }();

        KeyData& key_data = key_set.key_data[key].emplace_back();
        key_data.callbacks.emplace_back(callback);
        key_data.custom_data = custom_data;
        key_data.requires_modifier_keys = true;

        for (const auto& modifier_key : modifier_keys)
        {
            if (modifier_key != ModifierKey::MOD_KEY_START_OF_ENUM)
            {
                key_data.required_modifier_keys.emplace_back(modifier_key);
            }
        }
    }

    auto Handler::get_events() -> std::vector<KeySet>&
    {
        return m_key_sets;
    }

    auto Handler::get_allow_input() -> bool
    {
        return m_allow_input;
    }

    auto Handler::set_allow_input(bool new_value) -> void
    {
        m_allow_input = new_value;
    }
} // namespace RC::Input
