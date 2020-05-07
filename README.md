## TimeVR

As this is a C++ Unreal project, you will need to install Visual Studio as well as the Unreal Engine. [This guide](https://docs.unrealengine.com/en-US/Programming/Development/VisualStudioSetup/index.html) should show you which options to select when installing Visual Studio (specifically, you need to check the 'Game development with C++' option). 

- Unreal Engine version: 4.24.3  
- Visual Studio version: 16.5.4

#### Info & resources

[Project style guide](https://github.com/Allar/ue4-style-guide#structure) (no pressure to follow this exactly, but we should aim to follow this where possible).

For the moment you will need to build the project locally in order to play, but later on we can share downloadable releases to this repository which you can download and play.

#### Building the project

- Make sure you have the matching Unreal Engine version installed.
- Pull the latest changes using git
- Before opening the TimeVR.uproject file, right click `TimeVR.uproject` and select `generate Visual Studio project files` (if you don't see this option, something is not right with your Unreal Engine/Visual Studio installation)
- Open the new `TimeVR.sln` file which was created during the last step, it should open visual studio
- In visual studio, hit F7, or navigate to `Build > Compile`
- Once this completes, you can open the `TimeVR.uproject` file, which will open the Unreal Engine project

#### Contributing to the project

If you are familiar with Git, feel free to submit PRs with any changes or addition. If not, someone in the channel should be able to show you how to get started. Finally, feel free to simply share assets ect in the channel if that's preferred.
