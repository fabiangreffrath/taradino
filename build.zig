const std = @import("std");
//const Build = std.build;

pub fn build(b: *std.Build) void {
    //const target = b.standardTargetOptions(.{});
    //const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "rott",
        //.optimize = optimize,
        //.target = target,
        .optimize = b.standardOptimizeOption(.{}),
        .target = b.standardTargetOptions(.{}),
    });
    exe.addCSourceFiles(.{ .files = &.{
        "rott/byteordr.c",
        "rott/cin_actr.c",
        "rott/cin_efct.c",
        "rott/cin_evnt.c",
        "rott/cin_glob.c",
        "rott/cin_main.c",
        "rott/cin_util.c",
        "rott/dosutil.c",
        "rott/dukemusc.c",
        "rott/engine.c",
        "rott/fx_mixer.c",
        "rott/isr.c",
        "rott/m_misc2.c",
        "rott/modexlib.c",
        "rott/rt_actor.c",
        "rott/rt_battl.c",
        "rott/rt_build.c",
        "rott/rt_cfg.c",
        "rott/rt_com.c",
        "rott/rt_crc.c",
        "rott/rt_datadir.c",
        "rott/rt_debug.c",
        "rott/rt_dmand.c",
        "rott/rt_door.c",
        "rott/rt_draw.c",
        "rott/rt_err.c",
        "rott/rt_floor.c",
        "rott/rt_game.c",
        "rott/rt_in.c",
        "rott/rt_main.c",
        "rott/rt_map.c",
        "rott/rt_menu.c",
        "rott/rt_msg.c",
        "rott/rt_net.c",
        "rott/rt_playr.c",
        "rott/rt_rand.c",
        "rott/rt_scale.c",
        "rott/rt_sound.c",
        "rott/rt_sqrt.c",
        "rott/rt_stat.c",
        "rott/rt_state.c",
        "rott/rt_str.c",
        "rott/rt_ted.c",
        "rott/rt_util.c",
        "rott/rt_vid.c",
        "rott/rt_view.c",
        "rott/scriplib.c",
        "rott/watcom.c",
        "rott/winrott.c",
        "rott/w_wad.c",
        "rott/z_zone.c",
    }, .flags = &.{ "-D PACKAGE_STRING=\"rott 1.1.2\"", "-D PACKAGE_TARNAME=\"rott\"" } });

    exe.addIncludePath(.{ .path = "src" });

    exe.linkSystemLibrary("c");
    exe.linkSystemLibrary("sdl2");
    exe.linkSystemLibrary("SDL2_mixer");
    exe.linkSystemLibrary("SDL2_image");
    b.installArtifact(exe);

    const play = b.step("play", "Play the game");
    const run = b.addRunArtifact(exe);
    run.step.dependOn(b.getInstallStep());
    play.dependOn(&run.step);
}
