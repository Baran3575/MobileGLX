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
3. Launcher'da renderer olarak **MobileGLX / opengles3** seçin:
   - FCL (FoldCraftLauncher): Ayarlar → Renderer → MobileGLX
   - PojavLauncher / Zalith: renderer listesinde MobileGLX
4. Oyunu **Minecraft 26.3 vanilla veya Fabric 26.3** profiliyle başlatın.

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
