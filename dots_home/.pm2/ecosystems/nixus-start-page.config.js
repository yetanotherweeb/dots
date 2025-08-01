module.exports = {
  apps: [
    {
      name: "nixus-start-page",
      cwd: "/home/kamisaki/.local/share/nixus-start-page",
      script: "yarn",
      args: "start",
      interpreter: "/usr/bin/env",
      env: {
        NODE_ENV: "production",
      },
    },
  ],
};

