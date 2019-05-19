rFactor2 RaceStandings plugin
=============================

This version made by knackko "CedricH" is based on Cosimo devs but instead of exporting standings in one csv file, it exports standings in 2 files in order to admins to recreate the grid and lap gap after a server crash during an endurance round for example.

== What is this? ==

RaceStandings is an rFactor 2 plugin.
It is most useful when you are hosting races on a dedicated rF2 server.

== What does it do? ==

It exports the full race standings to ini files every time the leader completes a lap.
It activates only in Race sessions and writes two files each lap in UserData\Log\Results:
- editgrid_<lapnum>.ini
Editgrid contains /editgrid commands in order to recreate the grid respecting the standing at lap <lapnum>. Call this file with /batch editgrid_<lapnum>.ini command in rF2 during the warmup.

- changelaps_<lapnum>.ini
Changelaps contains /changelaps commands in order to recreate the lap gap between each driver respecting the gaps at lap <lapnum>. Call this file with /batch changelaps_<lapnum>.ini command in rF2 at the beginning of the race session.

== Why? ==

Because it sucks when a dedicated server crashes and there's no way to reconstruct the race situation from an official source.
This plugin will export the full race standings every time the leader completes a lap.
This could be a very long time for tracks like the Nordschleife, but this is how it works today.

== Installation ==

Copy the RaceStandings_x64.dll into your rFactor2 plugins folder,
most likely a directory like this:

  "C:\Program files (x86)\rFactor2\Bin64\Plugins"

== For feedback ==
A pull request has been done to Cosimo plugin dev.

Think that feedback on bugs, feature requests, generous donations :)
etc... are appreciated, in Cosimo original thread:

  https://forum.studio-397.com/index.php?threads/race-standings-dedicated-server-plugin.52705/

--

Knackko
