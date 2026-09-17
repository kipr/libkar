# Cross-compile toolchain for the Wombat (aarch64, Debian 13 / Qt 6).
# Pinned to Debian 13 to match the Wombat's OS and its Qt 6 version.
FROM debian:13.6 AS toolchain

RUN dpkg --add-architecture arm64 \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        cmake \
        g++-aarch64-linux-gnu \
        gcc-aarch64-linux-gnu \
        make \
        qt6-base-dev:arm64 \
    && rm -rf /var/lib/apt/lists/*

# Devcontainer: the toolchain plus interactive dev tools and agent CLIs.
# Selected with `--target dev`; see .devcontainer/README.md.
FROM toolchain AS dev

# file: inspect cross-compiled binaries (`file lib/libkar.so`).
# nodejs/npm: needed to install the Codex CLI.
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        bubblewrap ca-certificates curl fd-find file fish fzf git jq \
        nodejs npm openssh-client ripgrep wget \
    && ln -s "$(command -v fdfind)" /usr/local/bin/fd \
    && rm -rf /var/lib/apt/lists/*

RUN npm install -g @openai/codex \
    && codex --version

RUN groupadd --gid 1000 code \
    && useradd --uid 1000 --gid 1000 --create-home --shell /usr/bin/fish code

USER code
# Agent state lives in named volumes; a new volume inherits this ownership.
# CLAUDE_CONFIG_DIR also moves ~/.claude.json inside the volume.
ENV CLAUDE_CONFIG_DIR=/home/code/.claude \
    CODEX_HOME=/home/code/.codex \
    PATH=/home/code/.local/bin:$PATH
RUN mkdir -p "$CLAUDE_CONFIG_DIR" "$CODEX_HOME" \
    && curl -fsSL https://claude.ai/install.sh | bash \
    && claude --version

# Builder image used by the README's `docker run` build (default target, keep last).
FROM toolchain AS builder

RUN groupadd --gid 1000 kipr \
    && useradd --uid 1000 --gid 1000 kipr

USER kipr:kipr
