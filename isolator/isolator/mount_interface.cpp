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

#include <glog/logging.h>

#include <process/collect.hpp>
#include <process/io.hpp>
#include <process/subprocess.hpp>

#include "common/status_utils.hpp"

#include "slave/containerizer/mesos/isolators/docker/volume/MountManager.hpp"
#include "../libstor-c.h"

namespace io = process::io;

using std::string;
using std::tuple;
using std::vector;

using process::Failure;
using process::Future;
using process::Owned;
using process::Subprocess;

namespace mesos {
namespace slave {
namespace docker {
namespace volume {

Try<Owned<MountManagerClient>> MountManagerClient::create(
    const std::string& configfile)
{
  result libstorageClient = new_client(configfile);

  if (libstorageClient.err) {
    std::string err = libstorageClient.err;
    return Error("libstor-c: new_client error: " + err);
  }

  return Owned<MountManagerClient>(
    new MountManagerClient(configfile, *((h*)libstorageClient.val));
}

MountManagerClient::~MountManagerClient()
{
  // terminate libstorage client
  close(libstorageClientId);
  libstorageClientId = 0;
}

Try<std::string> MountManagerClient::mount(
    const std::string& serviceId,
    const std::string& volumeId)
{
  result libstorageAttach =
    mount(libstorageClientId, serviceId.c_str(), volumeId.c_str());

  if (libstorageAttach.err) {
    return Error("libstor-c: mount error: " + std::str(libstorageAttach.err));
  }

  volume_attachment* pva = (volume_attachment*)libstorageAttach.val;
  return std::string>(pva->mount_point);
}


Try<bool> MountManagerClient::unmount(
  const std::string& serviceId,
  const std::string& volumeId)
{
  result libstorageDetach =
    unmount(libstorageClientId, serviceId.c_str(), volumeId.c_str());

  if (libstorageDetach.err) {
    return Error("libstor-c: unmount error: " +
      std::str(libstorageDetach.err));
  }

  return true;
}

} // namespace volume {
} // namespace docker {
} // namespace slave {
} // namespace mesos {
