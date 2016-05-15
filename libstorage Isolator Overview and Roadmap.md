### libstorage Isolator Overview and Roadmap

---

## Persistent Storage for Mesos

Certain classes of applications, such as databases, require long term access to persistent storage.

---

## Issue and Workarounds

Techniques are available to run these types of workloads on Mesos, but sometimes the methods involved are essentially work-arounds that amount to using storage *not managed* by Mesos

---

### Workaround: Use direct attached storage

- Locks your workload to a single cluster node after first run, defeating an essential benefit of a cluster scheduler - the dynamic assignment of workloads to any, or the most appropriate, cluster node

---

### Workaround: Use external storage such as NFS from within the application

- Mesos is oblivious to this use of storage and cannot deliver in its role as a data center OS, offering centralized management and audit
    - This violates a key best practice premise of 12 factor apps by requiring maintenance of run time platform configuration inside the app itself

---

### Better solution

- Use the new libstorage Isolator

---

## What is the libstorage Isolator Module

Mesos has a rich facility to support plugins that add features.

The libstorage Isolator is a *module*, which is a binary plugin for Mesos.

In particular, it is an *isolator* module, which is a module that runs on agent cluster nodes and can interact with resource usage

---

## Features of the Mesos libstorage Isolator

- Manages external storage - storage that is network attachable to multiple cluster nodes,  
- Allows an application workload to be configured with a declarative statement describing its storage needs
    - This declaration is managed by Mesos. For example it can be done using the Marathon API.

---

##  Manages full volume mount lifecycle

- Allows a volume to be composed or allocated  and formatted from a pool upon first task run.
- Allows subsequent task runs to re-attach to the volume, even if the task runs on a different cluster node each time

---

## Supports Many storage types

- AWS EC2 EBS
- EMC ScaleIO
- EMC XtremIO
- many others too including NFS

Some of these storage types allow declarative specification of a rich set of storage attributes such as IOPs

---

## The libstorage Storage Interface

libstorage is a storage orchestration server.

---
