# MetaRenderer 文档

[English DOC](Renderer.md)

# 兼容性

|        引擎            |      |
|        ----              | ---- |
| GoldSrc_blob   (< 4554)  | -    |
| GoldSrc_legacy (< 6153)  | -    |
| GoldSrc_new    (8684 ~)  | √    |
| SvEngine       (8832 ~)  | √    |

* 警告: 该插件与ReShade不兼容.

# GPU 需求

|       厂商      |           Intel                  |          英伟达                  |          AMD               |
|       ----      |           ----                   |           ----                  |           ----             |
|      最低       | Intel Haswell 系列 HD4600         |      Geforce GTX 650            |  AMD Radeon HD 7000 系列 |
|     推荐        |           ----                   |      Geforce GTX 1060 或更高     |   AMD Radeon RX 560 或更高        |

* 运行该插件推荐使用拥有最少4GB独立显存的GPU，否则可能会出现显存带宽瓶颈导致帧数过低，或显存不足导致游戏崩溃等问题。


### 功能

## HDR

HDR (高动态范围) 模拟了超出显示器所能显示的亮度范围，将大于0-255的色彩范围映射到0-255的范围内，并且在超出100%亮度的地方施加Bloom（辉光）特效。

### 控制台参数

`r_hdr` 开启/关闭HDR后处理. 推荐值 : 1

`r_hdr_blurwidth` 设置HDR的模糊强度. 推荐值 : `0.0 ~ 0.1`

`r_hdr_exposure` 控制HDR的曝光程度. 推荐值 : `0.0 ~ 1.2`

`r_hdr_darkness` 控制HDR的明暗程度. 推荐值 : `0.0 ~ 1.6`

`r_hdr_adaptation` 控制HDR的明暗适应速度. 推荐值 : `0 ~ 50`

## 水面渲染

水面将拥有简单的反射和折射效果

水面可选择“可反射”和“传统”两种渲染模式

“可反射的水面”会实时反射水面上的物体，并折射水面下的物理。该功能需要渲染整个世界两次，所以有一定的性能开销，请根据掉帧严重程度自行斟酌是否开启！

“传统水面”则只会使用基础纹理进行渲染，就像原版GoldSrc中的一样。

反射等级和渲染参数可以使用[env_water_control](RendererCN.md#env_water_control)进行配置。

### 控制台参数

`r_water` 设为1启用“可反射的水面”。

## 逐对象阴影

逐对象阴影只会由模型进行投射 (玩家, 怪物, 武器盒, 尸体, 其他一些模型), 并且只会投射在固体表面. 逐对象阴影是实时计算的, 所以有一定的性能开销，请根据掉帧严重程度自行斟酌是否开启！

* 动态阴影有时候会穿过它本不该穿过的墙和地面，从而暴露玩家或NPC的位置。

### 控制台参数

`r_shadow` 设为1启用逐对象阴影

`r_shadow_angles` 控制阴影投射的角度, 以PitchYawRoll的格式. 举例： `r_shadow_angles "90 0 0"`

`r_shadow_color` 控制阴影的颜色, 以 RGBA8 的格式. 举例： `r_shadow_color "0 0 0 128"`

`r_shadow_distfade` 控制阴影开始淡出的距离，以及阴影的最大投射距离，单位为游戏内的距离单位. 举例：`r_shadow_distfade 64 128`

`r_shadow_lumfade` 控制阴影开始淡出的环境亮度, 以及阴影允许投射的最小环境亮度, 必须在 0 ~ 255 之间. 举例 `r_shadow_lumfade 64 32`

`r_shadow_high_distance` 该距离内的实体使用高质量的阴影贴图. 举例： `r_shadow_high_distance 400`

`r_shadow_high_scale` 控制渲染高质量的阴影贴图时的模型的缩放大小，缩放大小越大阴影精度越高，但是太大会导致阴影出错。举例： `r_shadow_high_scale 4`

`r_shadow_medium_distance` 该距离内的实体使用中等质量的阴影贴图. 举例： `r_shadow_medium_distance 800`

`r_shadow_medium_scale` 控制渲染中等质量的阴影贴图时的模型的缩放大小，缩放大小越大阴影精度越高，但是太大会导致阴影出错。举例： `r_shadow_medium_scale 2`

`r_shadow_low_distance` 该距离内的实体使用低质量的阴影贴图. 举例： `r_shadow_low_distance 1200`

`r_shadow_low_scale` 控制渲染低质量的阴影贴图时的模型的缩放大小，缩放大小越大阴影精度越高，但是太大会导致阴影出错。举例： `r_shadow_low_scale 0.5`

## 屏幕空间环境光遮蔽

SSAO （屏幕空间环境光遮蔽）是一种在后处理阶段为场景添加环境光遮蔽阴影的特效。

该功能参考了 [HBAO or Horizon-Based-Ambient-Occlusion](https://github.com/nvpro-samples/gl_ssao).

### 控制台参数

`r_ssao` 设为1启用SSAO

`r_ssao_intensity` 控制了SSAO阴影的强度

`r_ssao_radius` 控制了SSAO阴影的采样半径

`r_ssao_blur_sharpness` 控制了SSAO阴影的锐利程度

`r_ssao_bias` 用来在圆滑的曲面上消除不应该产生的SSAO阴影

## 延迟着色渲染管线 和 动态灯光

[延迟着色](https://en.wikipedia.org/wiki/Deferred_shading) 渲染管线被用来渲染不透明物体

并且引入了使用 [Blinn-Phong](https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model) 模型进行计算的实时光照

### 控制台参数

`r_light_dynamic` 设为1启用 延迟着色渲染管线 和 动态灯光

`r_flashlight_cone` 控制手电筒圆锥光束的圆锥夹角cosine值，越接近1则夹角越小，越接近0则夹角越大

`r_flashlight_distance` 控制手电筒的最大照明距离

`r_flashlight_ambient` 控制手电筒的环境光强度

`r_flashlight_diffuse` 控制手电筒的漫反射强度

`r_flashlight_specular` 控制手电筒的高光反射强度

`r_flashlight_specularpow` 控制手电筒的高光反射强度

`r_dynlight_ambient` 控制动态光源的环境光强度

`r_dynlight_diffuse` 控制动态光源的漫反射强度

`r_dynlight_specular` 控制动态光源的高光反射强度

`r_dynlight_specularpow` 控制动态光源的高光反射强度

## 屏幕空间反射

屏幕空间反射（下称SSR） 对屏幕空间中存在的物体进行实时反射。

只有被高光贴图标记了的固体表面 (在`/maps/[map name]_detail.txt`中以`_SPECULAR` 结尾的贴图)才会启用屏幕空间反射

`_SPECULAR`高光贴图的GREEN（绿色）通道代表了反射强度. 0 = 没有反射, 1 = 完全反射.

* 屏幕空间反射只有在 `r_light_dynamic` 为 1 时生效.

### 控制台参数

`r_ssr` 设为1启用屏幕空间反射

`r_ssr_ray_step` 控制光线步进的迭代步长。 举例： `r_ssr_ray_step 5.0`

`r_ssr_iter_count` 控制光线步进的最大迭代次数。 举例：`r_ssr_iter_count 64`

`r_ssr_distance_bias` 光线步进命中的判定距离。 举例： `r_ssr_distance_bias 0.2`

`r_ssr_adaptive_step` 设为1启用自适应迭代步长，可提升迭代性能。 举例： `r_ssr_adaptive_step 1`

`r_ssr_exponential_step` 设为1启用指数迭代步长，可提升迭代性能。 举例：`r_ssr_exponential_step 1`

`r_ssr_binary_search` 设为1启用二分搜索加速，可提升迭代性能。 举例： `r_ssr_binary_search 1`

`r_ssr_fade` 控制SSR效果贴近屏幕边缘时的淡出效果。举例： `r_ssr_fade "0.8 1.0"`

## 细节贴图

细节贴图是一种将高分辨率外部图片 （支持格式: BMP, TGA, DDS, JPG, PNG）与基础贴图混合来提升纹理细节的效果。

`r_detailtextures` 设为1启用细节贴图、法线贴图、视差贴图和高光贴图。

贴图列表会自动从文件 `/maps/[map name]_detail.txt` 中加载，以 `_DETAIL` 为后缀的贴图会被视为该基础贴图的细节贴图（如果基础贴图没有任何后缀则默认视为细节贴图）。

列表中指定的细节贴图文件会从 `/Sven Co-op/svencoop_(addon,downloads)/gfx/detail/` 和 `/Sven Co-op/svencoop/renderer/texture` 中加载（支持格式: BMP, TGA, DDS, JPG, PNG）。

### 法线贴图

法线贴图是一种使用外部贴图作用于特定固定表面，以改变其法线朝向的一种效果。

贴图列表会自动从文件 `/maps/[map name]_detail.txt` 中加载, 以 `_NORMAL` 为后缀的贴图会被视为该基础贴图的法线贴图。

列表中指定的法线贴图文件会从 `/Sven Co-op/svencoop_(addon,downloads)/gfx/detail/` 和 `/Sven Co-op/svencoop/renderer/texture` 中加载（支持格式: BMP, TGA, DDS, JPG, PNG）。

* 法线贴图只会改变固体表面的法线朝向, 因此只在被动态光源和手电筒照亮的表面起作用。

* 法线贴图只有在 `r_detailtextures` 和 `r_light_dynamic` 都设为 1 时有效。

### 视差贴图

视差贴图是一种使用外部贴图作用于特定固定表面，改变其视觉深度以营造一种凹陷突起的视觉效果。

贴图列表会自动从文件 `/maps/[map name]_detail.txt` 中加载, 以 `_PARALLAX` 为后缀的贴图会被视为该基础贴图的视差贴图。

列表中指定的视差贴图文件会从 `/Sven Co-op/svencoop_(addon,downloads)/gfx/detail/` 和 `/Sven Co-op/svencoop/renderer/texture` 中加载（支持格式: BMP, TGA, DDS, JPG, PNG）。

* `r_wsurf_parallax_scale` 控制视差(凹陷/突起)效果的最大强度（果为负则改变凹陷/突起的方向）。

* 视差贴图只有在 `r_detailtextures` 设为 1 时有效。

### 高光贴图

高光贴图是一种使用外部贴图作用于特定固定表面，以增强其高光反射强度的效果。

贴图列表会自动从文件 `/maps/[map name]_detail.txt` 中加载, 以 `_SPECULAR` 为后缀的贴图会被视为该基础贴图的高光贴图。

列表中指定的视差贴图文件会从 `/Sven Co-op/svencoop_(addon,downloads)/gfx/detail/` 和 `/Sven Co-op/svencoop/renderer/texture` 中加载（支持格式: BMP, TGA, DDS, JPG, PNG）。

* 高光贴图的红色分量（RGB的R）控制高光反射强度, 绿色分量控制SSR（屏幕空间反射）的强度。

* 蓝色分量未使用。

* 高光贴图只有在 `r_detailtextures` 设为 1 时有效。

## 卡通渲染 / 描边  / 边缘光 / 刘海阴影 / 头发高光

为了给指定的模型增加卡通渲染 / 描边  / 边缘光 / 刘海阴影 / 头发高光的效果，

你需要在`[modelname].mdl`模型的同目录下创建 `[modelname]_external.txt`文件，文件应包含以下内容：

```
{
    "classname" "studio_texture"
    "basetexture" "*"
    "flags" "STUDIO_NF_CELSHADE"
}
{
    "classname" "studio_texture"
    "basetexture" "face.bmp"
    "flags" "STUDIO_NF_CELSHADE_FACE"
}
{
    "classname" "studio_texture"
    "basetexture" "hair.bmp"
    "flags" "STUDIO_NF_CELSHADE_HAIR"
}
{
    "classname" "studio_efx"
    "flags" "EF_OUTLINE"
}
```

来为 `[modelname].mdl` 模型启用上述特效。

需要注意的是 `face.bmp` 和 `hair.bmp` 应修改为 `[modelname].mdl` 模型中真正的面部和头发贴图名。

或者参考 `Build\svencoop_addon\models\player\GFL_HK416\GFL_HK416_external.txt` 中提供的示例文件。

卡通渲染的参数会优先使用 `[modelname]_external.txt` 中的studio_celshade_control键值对：（举例）

```
{
    "classname" "studio_celshade_control"
    "celshade_midpoint" "-0.1"
    "celshade_softness" "0.05"
    "celshade_shadow_color" "160 150 150"
    "outline_size" "3.0"
    "outline_dark" "0.5"
    "rimlight_power" "5.0"
    "rimlight_smooth" "0.1"
    "rimlight_smooth2" "0.0 0.3"
    "rimlight_color" "40 40 40"
    "rimdark_power" "5.0"
    "rimdark_smooth" "0.1"
    "rimdark_smooth2" "0.0 0.3"
    "rimdark_color" "50 50 50"
    "hair_specular_exp" "256"
    "hair_specular_exp2" "8"
    "hair_specular_intensity" "0.3 0.3 0.3"
    "hair_specular_intensity2" "0.12 0.12 0.12"
    "hair_specular_noise" "500 600 0.004 0.005"
    "hair_specular_noise2" "100 120 0.006 0.0.005"
    "hair_specular_smooth" "0.0 0.3"
    "hair_shadow_offset" "0.3 -0.3"
}
```

如果键值对不存在，则使用对应的控制台参数。

### 控制台参数

`r_studio_celshade` 设为 1 启用卡通渲染 / 描边  / 边缘光 / 刘海阴影 / 头发高光。

`r_studio_celshade_midpoint` 和 `r_studio_celshade_softness` 控制卡通渲染阴影的柔软程度。

`r_studio_celshade_shadow_color` 控制卡通渲染阴影的颜色。

`r_studio_outline_size` 控制描边的大小。

`r_studio_outline_dark` 控制描边的颜色。

`r_studio_rimlight_power` 控制亮侧边缘光的强度。

`r_studio_rimlight_smooth` 控制亮侧边缘光的柔软程度。

`r_studio_rimlight_smooth2` 控制亮侧边缘光的在黑暗环境下的淡出表现。

`r_studio_rimlight_color` 控制亮侧边缘光的颜色。

`r_studio_rimdark_power` 控制暗侧边缘光的强度。

`r_studio_rimdark_smooth` 控制暗侧边缘光的柔软程度。

`r_studio_rimdark_smooth2` 控制暗侧边缘光的在黑暗环境下的淡出表现。

`r_studio_rimdark_color` 控制暗侧边缘光的颜色。

`r_studio_hair_specular_exp` 控制头发主高光照亮的区域大小，hair_specular_exp的值越大，照亮的区域越小。

`r_studio_hair_specular_noise` 控制头发主高光的噪声值，噪声用于生成类似WWW的波纹形状

`r_studio_hair_specular_intensity` 控制头发主高光的RGB强度

`r_studio_hair_specular_exp2` 控制头发次高光照亮的区域大小，hair_specular_exp2的值越大，照亮的区域越小。

`r_studio_hair_specular_noise2` 控制头发次高光的噪声值，噪声用于生成类似WWW的波纹形状

`r_studio_hair_specular_intensity2` 控制头发次高光的RGB强度

`r_studio_hair_specular_smooth` 控制头发高光在黑暗环境下的淡出表现。

`r_studio_hair_shadow_offset` 控制刘海阴影在屏幕空间下相对水平和垂直方向的偏移。

## 顶点缓冲对线 (又称 VBO) 批量绘制 优化

固体、模型、印花和SPR都使用VBO进行渲染，将渲染所需的顶点数据等信息提前保存在显存中，并且对能够合并绘制的物体进行的合并，极大提升了绘制效率。

## 顺序无关透明混合渲染 (又称OIT混合)

透明像素被保存在GPU链表中并由GPU进行排序。

添加启动参数 `-oitblend` 来启用OIT混合 (默认不启用)

* 警告：该功能在透明物体较多的情况下会严重影响性能。

## 抗锯齿

`r_fxaa` 设为1启用快速近似抗锯齿 (FXAA).

* 由于多重采样抗锯齿（MSAA）与延迟渲染不兼容，因此MSAA被彻底移除。

## Gamma矫正

原版GoldSrc的Gamma矫正在初始化和贴图加载阶段就已完成，而新的Gamma矫正由shader在运行时完成，允许你在游戏中途修改gamma、texgamma等数值。

所有贴图会从gamma空间转换为线性空间后再进行光照计算

### 控制台参数

`gamma` 控制最终输出画面的gamma值, 用于将颜色从线性空间转换至gamma空间。

`texgamma` 用于将贴图的颜色从gamma空间转换至线性空间。

`lightgamma` 用于将光照贴图的颜色从gamma空间转换至线性空间。

`brightness` 用于偏移lightgamma来让光照贴图的结果更亮

## 其他

`r_wsurf_sky_occlusion` 1 / 0 : 设为1时, 被"sky"贴图遮挡的物体将会不可见。

`r_wsurf_zprepass` 1 / 0 : 设为1时启用Z-Prepass优化。