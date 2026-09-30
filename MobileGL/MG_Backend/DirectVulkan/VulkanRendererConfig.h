// MobileGL - MobileGL/MG_Backend/DirectVulkan/Renderer/VulkanRendererConfig.h
// Copyright (c) 2025-2026 MobileGL-Dev
// Licensed under the GNU Lesser General Public License v3.0:
//   https://www.gnu.org/licenses/gpl-3.0.txt
//   https://www.gnu.org/licenses/lgpl-3.0.txt
// SPDX-License-Identifier: LGPL-3.0-only
// End of Source File Header

#pragma once
#include "Config.h"

namespace MobileGL::MG_Backend::DirectVulkan {
    struct VulkanRendererConfig {
        // Fallback CPU pipeline depth used when the MOBILEGL_MAGMA_FRAMESINFLIGHT env var is
        // unset/invalid. A deeper pipeline lets the CPU run further ahead of the GPU, hiding
        // per-frame GPU-completion latency. Whatever value is chosen (env or this fallback) is
        // only a request: VulkanRenderer::Initialize clamps it down to the surface's maxImageCount
        // (and never below 2), since not every driver allows that many swapchain images.
        Uint32 MaxFramesInFlight = 3;
        String AppName = "MobileGL-VulkanRenderer";
        MobileGL::Version Version = MG_Config::CoreVersion;
        Uint64 CacheVersion = MG_Config::CacheVersion;
        Uint32 SurfaceWidth = 1;
        Uint32 SurfaceHeight = 1;
        // The app's eglSwapInterval; empty until it asks. Window renderers only - see
        // VulkanRenderer::SetSwapInterval.
        Optional<Int> SwapInterval;
        Bool DisablePipelineCache = false;
        // Disk persistence for the VkPipelineCache (MOBILEGL_MAGMA_PIPELINE_CACHE_DIR).
        // Empty = auto-resolve (Android host-app cache dir, memory-only elsewhere);
        // "0"/"off" disables even the auto path. A 32-char lowercase hex device UUID
        // scopes the file name so a blob can never cross drivers.
        String PipelineCacheDir;
        String PipelineCacheUUIDHex;
        Uint32 PipelineCacheDriverVersion = 0;
#if MOBILEGL_LOG_ACTIVE_LEVEL <= MOBILEGL_LOG_LEVEL_DEBUG
        Bool EnableValidationLayers = true;
#else
        Bool EnableValidationLayers = false;
#endif
    };
} // namespace MobileGL::MG_Backend::DirectVulkan
