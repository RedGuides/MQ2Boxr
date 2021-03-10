## MQ2Boxr

This plugin provides a command to control boxes running CWTN plugins,
KissAssist, and rgmercs. The idea is to have one common command, which can be
broadcast to all boxed characters, regardless of what they are running.

### Commands

| Command                         | Description
|---------------------------------|-------------
| `/boxr Pause`                   | Pause the character
| `/boxr Unpause`                 | Unpause the character
| `/boxr Camp`                    | Camp in one spot, assist the MA within camp radius, and return to camp after battle
| `/boxr Chase`                   | Chase the MA, and assist in battle
| `/boxr Manual`                  | Do not chase, do not return to camp. This state behaves a little bit different between different boxes, see [specifics](#specifics) below
| `/boxr CampRadius <N>`          | Sets camp radius to `N` (N.B. For CWTN, the new CampRadius will be saved)
| `/boxr RaidAssistNum <1\|2\|3>` | Toggles which Raid MA the character will assist
| `/boxr Debug <on\|off>`         | Turn on/off debug logging
| `/boxr Help`                    | Print help text

### Specifics

The following sections document how each boxr command maps to the different
plugins/macros.

#### CWTN plugins

The mapping to CWTN commands is very straight-forward

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/war mode assist`
| `Chase`              | `/war mode chase`
| `Manual`             | `/war mode manual`
| `CampRadius <N>`     | `/war campradius N`
| `RaidAssistNum <N>`  | `/war RaidAssistNum`

#### KissAssist

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `CampRadius <N>`     | `/campradius N`
| `RaidAssistNum <N>`  | `/switchma <Name of Raid MA #N> tank 1`

#### rgmercs

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/rg camphard`
| `Chase`              | `/rg chaseon`
| `Manual`             | `/rg chaseoff`<br/>`/rg campoff`
| `CampRadius <N>`     | `/rg autocampradius <N>`
| `RaidAssistNum <N>`  | `/rg AssistOutside 1`<br />`/rg OutsideAssistList <Name of Raid MA #N>`

**N.B.** The rgmercs adapter has undergone much less testing than the
CWTN and KissAssist ones.
