/*
 * Copyright 2019-2023 PixlOne
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LOGID_BACKEND_RAWDEVICE_H
#define LOGID_BACKEND_RAWDEVICE_H

#include <string>
#include <vector>
#include <shared_mutex>
#include <atomic>
#include <future>
#include <set>
#include <list>

#include "EventHandler.h"

namespace logid::backend::raw {
    class DeviceMonitor;

    class IOMonitor;

    class RawDevice {
    public:
        static constexpr int max_data_length = 32;
        typedef std::list<RawEventHandler>::const_iterator EvHandlerId;

        struct dev_info {
            int16_t vid;
            int16_t pid;
        };

        RawDevice(std::string path,
                  const std::shared_ptr<DeviceMonitor>& monitor);

        ~RawDevice() noexcept;

        [[nodiscard]] const std::string& rawPath() const;

        [[nodiscard]] const std::string& name() const;

        [[maybe_unused]]
        [[nodiscard]] int16_t vendorId() const;

        [[nodiscard]] int16_t productId() const;

        static std::vector<uint8_t> getReportDescriptor(const std::string& path);

        static std::vector<uint8_t> getReportDescriptor(int fd);

        [[nodiscard]] const std::vector<uint8_t>& reportDescriptor() const;

        void sendReport(const std::vector<uint8_t>& report);

        EvHandlerId addEventHandler(RawEventHandler handler);

        void removeEventHandler(EvHandlerId id);

    private:
        void _readReports();

        std::atomic_bool _valid;

        const std::string _path;
        const int _fd;
        const dev_info _dev_info;
        const std::string _name;
        const std::vector<uint8_t> _report_desc;

        std::shared_ptr<IOMonitor> _io_monitor;

        std::list<RawEventHandler> _event_handlers;
        std::shared_mutex _event_handler_mutex;

        void _handleEvent(const std::vector<uint8_t>& report);
    };
}

#endif //LOGID_BACKEND_RAWDEVICE_H
