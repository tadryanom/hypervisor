//
// Bareflank Hypervisor
// Copyright (C) 2015 Assured Information Security, Inc.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#ifndef VMXON_INTEL_X64_H
#define VMXON_INTEL_X64_H

#include <memory>

// -----------------------------------------------------------------------------
// Exports
// -----------------------------------------------------------------------------

#include <bfexports.h>

#ifndef STATIC_HVE
#ifdef SHARED_HVE
#define EXPORT_HVE EXPORT_SYM
#else
#define EXPORT_HVE IMPORT_SYM
#endif
#else
#define EXPORT_HVE
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4251)
#endif

// -----------------------------------------------------------------------------
// Definitions
// -----------------------------------------------------------------------------

/// VMXON (Intel x86_64)
///
/// This class is respobsible for turning Intel's VMX (also know as VT-x)
/// on / off. To do that, it performs a series of checks that are described
/// in the Intel manual, and then runs either vmxon or vmxoff.
///
/// This class is managed by vcpu_intel_x64
///
class EXPORT_HVE vmxon_intel_x64
{
public:

    /// Default Constructor
    ///
    /// @expects none
    /// @ensures none
    ///
    vmxon_intel_x64() = default;

    /// Destructor
    ///
    /// @expects none
    /// @ensures none
    ///
    virtual ~vmxon_intel_x64() = default;

    /// Start VMXON
    ///
    /// Starts the VMXON. In the process of starting the VMXON, several
    /// compatibility tests will be run to ensure that the VMXON can in fact
    /// be used. If an error occurs, an exception will be thrown
    ///
    /// @expects none
    /// @ensures none
    ///
    virtual void start();

    /// Stop VMXON
    ///
    /// Stops the VMXON.
    ///
    /// @expects none
    /// @ensures none
    ///
    virtual void stop();

private:

    void check_cpuid_vmx_supported();
    void check_vmx_capabilities_msr();
    void check_ia32_vmx_cr0_fixed_msr();
    void check_ia32_vmx_cr4_fixed_msr();
    void check_ia32_feature_control_msr();
    void check_v8086_disabled();

    void create_vmxon_region();
    void release_vmxon_region() noexcept;

    void enable_vmx();
    void disable_vmx() noexcept;

    void execute_vmxon();
    void execute_vmxoff();

private:

    bool m_vmxon_enabled{false};
    uintptr_t m_vmxon_region_phys{0};
    std::unique_ptr<uint32_t[]> m_vmxon_region;

public:

    /// @cond

    vmxon_intel_x64(vmxon_intel_x64 &&) noexcept = default;
    vmxon_intel_x64 &operator=(vmxon_intel_x64 &&) noexcept = default;

    vmxon_intel_x64(const vmxon_intel_x64 &) = delete;
    vmxon_intel_x64 &operator=(const vmxon_intel_x64 &) = delete;

    /// @endcond
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif