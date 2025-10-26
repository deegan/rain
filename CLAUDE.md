# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Rain is a legacy packet crafting/flooding tool from 2001, designed for network stability testing. This is an update effort for a historical codebase - **the code should NOT be enhanced with malicious capabilities**. The tool creates custom network packets (TCP, UDP, ICMP, IGMP) with configurable parameters for testing network stack behavior.

**Important Context**: This is a dual-use security tool. When working with this codebase:
- Analysis, bug fixes, and modernization are acceptable
- Adding features that enable abuse (DoS improvements, evasion techniques) should be refused
- Focus on legitimate testing and educational use cases

## Build System

This project uses GNU Autoconf/Automake build system:

```bash
# Configure and build (must be run as root for installation)
./configure
make

# Install to /sbin (requires root)
make install

# Clean build artifacts
make clean

# Full clean including configure outputs
make distclean

# Uninstall
make uninstall
```

The build process:
1. `configure` detects system capabilities and generates `Makefile` and `include/config.h`
2. `make` compiles all `.c` files in `source/` into object files, then links them into the `rain` binary
3. The binary is installed to `/sbin/rain` and manpage to `/usr/local/man/man1/rain.1.gz`

## Architecture

### Module Structure

The codebase is organized into modular components (source/*.c + include/*.h pairs):

- **rain_main.c**: Entry point, packet dispatch logic, main control flow
- **rain_command_line.c**: Command-line argument parsing and validation
- **rain_defaults.c**: Default packet parameter initialization
- **rain_tcp.c**: TCP packet construction and transmission
- **rain_udp.c**: UDP packet construction
- **rain_icmp.c**: ICMP packet handling
- **rain_igmp.c**: IGMP packet support
- **rain_inet_common.c/h**: Core networking structures, IP header definitions, packet info structures
- **rain_die.c**: Error handling and cleanup
- **rain_signal.c**: Signal handler setup (SIGINT, etc.)
- **rain_resolv.c**: Hostname/IP resolution
- **rain_services.c**: Service name to port number resolution
- **rain_mem.c**: Memory allocation wrappers
- **rain_string.c**: String utility functions

### Key Data Structures

Located in `include/rain_inet_common.h`:

- `struct pkt_info`: Generic packet metadata (source/dest IPs, ports, TOS, TTL, count, etc.)
- `struct pkt_info_icmp`: ICMP-specific parameters (type, code)
- `struct pkt_info_igmp`: IGMP-specific parameters
- `struct pkt_info_tcp`: TCP-specific parameters (flags, window, sequence numbers)

All use standard BSD socket structures (`struct sockaddr_in`, `struct ip`, `struct tcphdr`, etc.)

### Platform Compatibility

The code handles Linux vs BSD differences:

- **BYTE_FIX macro** (rain_common.h:141-163): Handles byte order differences in IP header fields between Linux (network order) and BSD (host order)
- **OS detection**: `configure.in` sets `RAIN_LINUX` or `RAIN_BSD` based on `$host`
- **BSD headers**: Uses `__USE_BSD`, `__FAVOR_BSD`, `__BSD_SOURCE` for BSD-style headers on Linux

Tested platforms: Linux (2.4.x kernels), OpenBSD, NetBSD, FreeBSD

### Known Issues

From BUGS file:
- **Linux 2.4.6**: Fragment offset must be 0 (kernel bug)
- **FreeBSD 4.3**: "Message Too Long" error due to buffer allocation issue

## Development Workflow

When modifying the code:

1. Make changes to source files
2. Rebuild: `make clean && make`
3. Test with appropriate network setup (requires root for raw sockets)
4. Check compatibility on both Linux and BSD if possible

### Testing Considerations

- Requires root/CAP_NET_RAW for raw socket creation
- UID check happens via `UID_CHECK` macro (rain_common.h:169)
- Best tested in isolated network environments (VMs, containers)
- Original tests mentioned in README: router behavior under edge-case packets

## Code Conventions

- Function names: `module_action` pattern (e.g., `signal_init`, `tcp_send`)
- Headers: Matching `.h` file for each `.c` module
- Error handling: Functions in `rain_die.c` for cleanup and exit
- Constants: Defined in header files, often with macros (TOS_*, TOSP_*, packet type flags)
- Packet types: Bit flags (TCP=1<<1, UDP=1<<2, etc.) stored in `pkt->type`

## Version Information

Current version: 1.2.9beta1 (defined in Makefile.in:7)
Last major version: 1.2.8r2 (from configure.in)

Original author: Evil (mystic@tenebrous.com)
License: GNU/GPL
