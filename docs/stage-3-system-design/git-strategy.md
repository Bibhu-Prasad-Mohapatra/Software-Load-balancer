# Git Branching & Workflow Strategy

To simulate a professional team environment, all commits will follow conventional commit messaging and be merged thoughtfully.

## 1. Branch Layout

- **`main`**: Production-ready code. Always stable, builds successfully, and tests pass.
- **`develop`**: Integration branch for new features.
- **`feature/*`**: Short-lived branches for specific milestones.

### Proposed Feature Branches for Implementation
1. `feature/m1-m2-simulators` - Backend and Client simulation tools.
2. `feature/m3-m5-core-lb` - Basic TCP binding, Epoll loop, Round Robin.
3. `feature/m6-m8-health` - Health-check loop and failure/recovery logic.
4. `feature/m9-m10-algo` - Least Connections integration and concurrency handling.
5. `feature/m11-m13-stability` - Logging, Signals (SIGINT), metrics.

## 2. Commit Message Convention

Commits will use prefixes to clarify intent:
- `feat:` A new feature (e.g., `feat: implement round robin algorithm`)
- `fix:` A bug fix (e.g., `fix: resolve socket leak on disconnect`)
- `docs:` Documentation updates
- `test:` Adding missing tests or correcting existing ones
- `perf:` Code optimizations (e.g., switching to epoll)
- `refactor:` Code changes that neither fix a bug nor add a feature

*Note: No dummy or empty commits will be made. Every commit will represent an actual step forward in the codebase.*
