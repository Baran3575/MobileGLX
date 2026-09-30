# MobileGLX — Minecraft Java Odaklı MobileGL Fork'u

MobileGLX, [MobileGL](https://github.com/MobileGL-Dev/MobileGL)'in Minecraft: Java Edition
odaklı fork'udur.

**Fork sebebi:** MobileGlues daha seyrek güncellenirken MobileGL çok daha aktif
geliştiriliyor. MobileGLX, MobileGL'in güncel çekirdeğini alıp Minecraft için
varsayılanları, paket kimliğini ve build altyapısını Minecraft'a göre ayarlar.

## Desteklenen Sürüm (şu an)

| Bileşen | Sürüm |
|---|---|
| Minecraft Java | **26.3** (Wilderness Bound, 15 Eylül 2026, protokol 777) |
| Yükleyici | Vanilla **+** Fabric |
| Fabric Loader | 0.19.5 |
| Fabric API | 0.161.0+26.3 |
| Java | 25 |
| Data pack formatı | 121.0 |
| Resource pack formatı | 97.1 |

> Not: Paper 26.3 hâlâ deneysel (alpha) kanalda; bu fork **vanilla + Fabric 26.3**'ü
> hedefler. Yeni MC sürümleri için `mobileGlxMcVersion` tek yerden büyütülür
> (`android-plugin/app/build.gradle.kts` en üstü).

## Kurulum (Android)

1. **Releases** sayfasından `MobileGLX-plugin-release-*.apk` dosyasını indirin
   (veya Actions → MobileGLX APK / MobileGLX Release artifact'larından alın).
2. APK'yı kurun (upstream MobileGL plugin'i ile yan yana durabilir;
   paket adı farklıdır: `top.mobileglx.plugin`).
3. Launcher'da renderer olarak **MobileGLX / opengles3** seçin.

### Zalith Launcher 2 (v2.6.1)

1. ZL2 v2.6.1'i kurun, `MobileGLX-plugin-release-*.apk` dosyasını kurun.
2. ZL2 → Ayarlar → Renderer → **MobileGLX** (V2 plugin olarak görünür;
   V1 fallback `fclPlugin`/`pojavEnv` de içerir, eski launcher'larda da çalışır).
3. Oyunu **Minecraft 26.3 vanilla veya Fabric 26.3** profiliyle başlatın.
   Plugin `minMCVer/maxMCVer = 26.3` bildirdiği için ZL2 onu 26.3 profillerinde listeler.
4. Backend: önce `DirectGLES` (varsayılan). Vulkan 1.2+ cihazda takılma olursa
   `DirectVulkan` + disk pipeline cache (otomatik) devreye girer.

Not: APK `arm64-v8a` içerir (gerçek cihazların neredeyse tamamı). x86_64
(emülatör) gerekiyorsa Actions → MobileGLX Release → Run workflow →
`abis=all` ile manuel build alın.

## Backend ve Varsayılanlar

| Ayar | Varsayılan | Neden |
|---|---|---|
| `MOBILEGL_BACKEND_TYPE` | `DirectGLES` | Çoğu Adreno/Mali cihazda en uyumlu yol |
| `MOBILEGL_DISABLE_TIMERQUERY` | açık (`1`) | F3/profiler çökmelerine karşı |
| `MOBILEGL_COHERENT_AS_FLUSH` | açık (`1`) | Flywheel/Create + Embeddium kalıcı-map uyumu |
| `MOBILEGL_RELAXED_SEMANTICS` | açık (`1`) | Sodium/Iris toleransı |
| `MOBILEGL_ESPRYT_USE_ANGLE` | kapalı | Gerekirse launcher'dan açın |
| `MOBILEGL_MAGMA_*` | upstream ile aynı | DirectVulkan alternatifi için |

Öneri: önce `DirectGLES` ile deneyin; sorun yaşarsanız launcher ayarlarından
`DirectVulkan` backend'ine geçin.

## Optimizasyonlar (MobileGLX, Java/Minecraft'a özel)

MobileGlues felsefesi (Minecraft'a özel varsayılanlar + kalıcı shader önbelleği),
MobileGL çekirdeği üzerinde:

- **Vulkan pipeline cache persist** (`MOBILEGL_MAGMA_PIPELINE_CACHE_DIR`):
  driver pipeline derlemesi (`vkCreateGraphicsPipelines`) dünya yükleme ve Iris
  shaderpack reload'daki ana takılmadır. İlk açılışta derlenen her şey paket-scope
  dosyaya yazılır (`magma_pipeline_<uuid>_<cachever>.bin`, header'da magic +
  CacheVersion + driver sürümü + cihaz UUID; 64 MiB cap). İkinci açılışta
  görülmüş pipeline'lar atlanır. Boş değer = otomatik: Android'de host uygulamanın
  cache dizini (`/proc/self/cmdline` → paket → `.../cache/mobileglx`, probe-write
  ile doğrulanır), diğer platformlarda memory-only. `0`/`off` = kapalı.
  Bozuk/uyumsuz blob ölümcül değildir: header doğrulanır, driver reddederse boş
  cache ile devam edilir.
- **Async shader derleme ayarları** (launcher UI'dan):
  `MOBILEGL_ASYNC_SHADER_COMPILE_THREADS` (`0` = otomatik, `min(4, big core)`) ve
  `MOBILEGL_ASYNC_OPTIMISTIC_SHADER_STATUS` (Iris'in program-sorgusuz yüzlerce
  shader derleyen gbuffer fazı için; link join'i her zaman doğru kalır).
- **Zaten çekirdekte var, Minecraft için açık gelenler**: `glUniform` bytes-equal
  dedupe (her frame aynı matris/sampler tekrarını backend'e iletmez),
  sampler/image unit dedupe, redundant state filtreleme (frontend equality
  early-out + backend shadow `memcmp`), async compile pool + adoption map
  (shaderpack burst tekrarları tek job'a iner).

## Build'ler (GitHub Actions)

| Workflow | Tetikleyici | Çıktı |
|---|---|---|
| `MobileGLX APK` (`.github/workflows/apk.yml`) | `dev`, `main`, `mc26.3*` push'ları + tag | plugin + trace APK (artifact) + emulator retrace |
| `MobileGLX Release` (`.github/workflows/release.yml`) | `mc26.3*` / `v*` tag push'u | GitHub **Release**'e plugin APK eklenir |

Release çıkarmak için:

```sh
git tag mc26.3-1
git push origin mc26.3-1
```

İmza notu: repo secret'larında `SIGNING_STORE_PASSWORD`, `SIGNING_KEY_ALIAS`,
`SIGNING_KEY_PASSWORD` varsa release anahtarıyla imzalanır; yoksa (fork/PR
build'leri) debug anahtarıyla imzalanır — APK her durumda kurulabilir.

## Geliştirme Notları

- Paket adı: `top.mobileglx.plugin` (trace: `top.mobileglx.plugin.trace`).
  Upstream `top.mobilegl.plugin` ile çakışmaz.
- Native lib adı değişmedi: `libMobileGL.so` (CMake hedefi aynı).
- Sürüm şeması upstream ile aynı tutulur (`26.08.<hash>`), sonuna `-mc26.3`
  suffix'i eklenir. `versionCode` geriye gitmesin diye Minor düşürülmedi.
- Yeni MC sürümü desteği eklemek: `mobileGlxMcVersion` değerini büyütün,
  `README_MobileGLX.md` tablosunu güncelleyin, tag'leyip release alın.
- Upstream `dev` ile senkron kalmak için: `git fetch upstream && git merge upstream/dev`
  (çakışma genelde `build.gradle.kts` üstündeki MobileGLX bloğunda olur).

## Lisans

Upstream ile aynı: **GNU LGPL v3.0** (`LICENSE` dosyasına bakın).
