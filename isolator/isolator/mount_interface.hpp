// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __MOUNT_INTERFACE_HPP__
#define __MOUNT_INTERFACE_HPP__

#include <string>

#include <process/future.hpp>
#include <process/owned.hpp>

#include <stout/hashmap.hpp>
#include <stout/nothing.hpp>
#include <stout/try.hpp>

namespace mesos {
namespace slave {
namespace docker {
namespace volume {

/**
 * Abstraction for libstorage Client.
 * libstorage is a c interface for the libstorage mount interface.
 * Because 'c' doesn't support namespaces so this encapsulates
 * the interface.
 */
class MountManagerClient
{
public:
  /**
   * Create a libstorage Client.
   */
  static Try<process::Owned<MountManagerClient>> create(
      const std::string& configfile);

  virtual ~MountManagerClient();

  /**
   * Performs a 'mount' and returns the path of the mount point. If
   * the 'options' field is specified, the 'mount' will do an implicit
   * creation of the volume if it does not exist.
   */
  virtual Try<std::string> mount(
    const std::string& serviceId,
    const std::string& volumeIdconst);

  /**
   * Performs an 'unmount'.
   */
  virtual Try<bool> unmount(
    const std::string& serviceId,
    const std::string& volumeIdconst);

protected:
  //MountManagerClient() {} // For creating mock object.

private:
  MountManagerClient(const std::string& _configfile, unsigned long long _h)
    : configfile(_configfile), libstorageClientId(_h) {}

  const std::string        configfile;
  const unsigned long long libstorageClientId;
};

} // namespace volume {
} // namespace docker {
} // namespace slave {
} // namespace mesos {

#endif // __MOUNT_INTERFACE_HPP__
