![gstreamer-plugin](docs/media/consume_stream.gif)

# Setup

- Clone this repository in the `Plugins` directory of your UE4 project:

```
git clone git@github.com:Simbotic/ue4-gst-plugin.git
```

- Update your `.uproject` adding the `Engine` as additional dependency and the `GStreamer` plugin:

```
...,
"Modules": [
	{
		"Name": "GST_Test",
		"Type": "Runtime",
		"LoadingPhase": "Default",
		"AdditionalDependencies": [
			"Engine"
		]
	}
],
"Plugins": [
	{
		"Name": "GStreamer",
		"Enabled": true
	},
],
...

```
- Add, in the constructors of the classes of your project: `Source/PROJECT_NAME.Target.cs` and `Source/PROJECT_NAMEEditor.Target.cs` the following:
```
bUseUnityBuild = false;
bUsePCHFiles = false;
```

# Usage

Blueprints:

![gst-plugin-ue4 blueprints](docs/media/gst_pipeline.png)

Details panel:

![gst-plugin-ue4 details panel](docs/media/details_panel.png)
