# libkar devcontainer

A Debian 13 container with the aarch64 cross-compile toolchain for the Wombat,
plus dev tools and the Claude Code and Codex CLIs.

## What's in the image

The image is the `dev` stage of the repo's root `Dockerfile`:

- `toolchain` stage (shared with the builder image): `cmake`, `make`,
  `gcc/g++-aarch64-linux-gnu`, `qt6-base-dev:arm64`.
- `dev` stage adds: fish, git, ripgrep, fd, fzf, jq, curl, wget, `file`,
  bubblewrap, Node.js (for Codex), the Codex CLI, and the Claude Code CLI.
- User `code` (UID 1000, home `/home/code`, shell fish).

The default target (`builder`) is unchanged, so the README's
`docker build -t libkar-builder .` still builds the plain builder image.

## Opening it

- VS Code: "Dev Containers: Reopen in Container".
- CLI: `devcontainer up --workspace-folder .`, then
  `devcontainer exec --workspace-folder . fish`.

Standalone image build (no devcontainer tooling):

```sh
docker build --target dev -t libkar-dev .
```

Rebuild the container after changing the `Dockerfile` or `devcontainer.json`.

## Building libkar inside the container

```sh
cmake -Bbuild -DCMAKE_TOOLCHAIN_FILE=toolchain/aarch64-linux-gnu.cmake .
cmake --build build -j "$(nproc)"
cd build && cpack   # produces libkar-<version>-Linux.deb (arm64)
```

The library lands in `lib/libkar.so`; `file lib/libkar.so` should report
`ARM aarch64`.

If `build/` was configured by the README's `docker run` build (source at
`/src`), CMake refuses to reuse it here because the source path differs
(`/workspaces/libkar`). Delete `build/` or use another build directory.

## Agent CLIs

Agent state lives in two named Docker volumes shared by every devcontainer on
this machine, so you log in once per machine:

| CLI | Volume | Mounted at |
|---|---|---|
| Claude Code | `claude-config` | `/home/code/.claude` |
| Codex | `codex-config` | `/home/code/.codex` |

First time only, inside the container:

```sh
claude          # follow the login prompt
codex login     # use device-code login if the browser callback can't reach the container
```

Smoke tests:

```sh
claude --version && claude -p "say hi"
codex --version && codex sandbox -- true
```

Host-level settings, skills, plugins and memories are not shared into the
container; project-level config (`.claude/`, `AGENTS.md`) comes with the
workspace. Containers share these volumes live, so sessions running in two
containers at once write the same files.

## Caveats

- `--security-opt=seccomp=unconfined` is set for the agents' bubblewrap
  sandbox. This disables Docker's syscall filter for this container.
- The sandbox still fails with only that option (`bwrap: Can't mount proc on
  /proc: Operation not permitted`), because Docker masks parts of `/proc`.
  Adding `--security-opt=systempaths=unconfined` to `runArgs` should fix it,
  but it also unmasks `/proc` and `/sys` paths for the whole container, so it
  is not enabled by default. Without it, run the agents with their sandbox
  off; the container is the isolation boundary.
- The image is x86_64 and cross-compiles; nothing built here runs in the
  container. Test binaries on the Wombat.
- Podman, or a Docker daemon with SELinux enabled, may deny access to the
  workspace mount. If so, add `--security-opt=label=disable` to `runArgs`.
