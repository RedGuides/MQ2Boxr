## MQ2Boxr

This plugin provides a command to control boxes running CWTN plugins,
KissAssist, MuleAssist, rgmercs, and Entropy. The idea is to have one common command, 
which can be broadcast to all boxed characters, regardless of what they are 
running.

### Commands

| Command                         | Description
|---------------------------------|-------------
| `/boxr Pause`                   | Pause the character
| `/boxr Unpause`                 | Unpause the character
| `/boxr Camp`                    | Camp in one spot, assist the MA within camp radius, and return to camp after battle
| `/boxr Chase`                   | Chase the MA, and assist in battle
| `/boxr Manual`                  | Do not chase, do not return to camp. This state behaves a little bit different between different boxes, see [specifics](#specifics) below
| `/boxr BurnNow`                 | Burn current target
| `/boxr RaidAssistNum <1\|2\|3>` | Toggles which Raid MA the character will assist. This command is not supported on the Emulator build
| `/boxr Debug <on\|off>`         | Turn on/off MQ2Boxr debug logging
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
| `BurnNow`            | `/war BurnNow`
| `RaidAssistNum <N>`  | `/war RaidAssistNum <N>`

#### KissAssist

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `BurnNow`            | `/burn on doburn`
| `RaidAssistNum <N>`  | `/switchma <Name of Raid MA #N> tank 1`

#### MuleAssist

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `BurnNow`            | `/burn`
| `RaidAssistNum <N>`  | `/changema <Name of Raid MA #N>`

#### AlsoKissAssist

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `BurnNow`            | `/burn on doburn`
| `RaidAssistNum <N>`  | `/switchma <Name of Raid MA #N>`

#### rgmercs

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/rg camphard`
| `Chase`              | `/rg chaseon`
| `Manual`             | `/rg chaseoff`<br/>`/rg campoff`
| `BurnNow`            | *Not supported*
| `RaidAssistNum <N>`  | `/rg AssistOutside 1`<br />`/rg OutsideAssistList <Name of Raid MA #N>`

#### Entropy

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/tie off` <br/>`/home set on`
| `Chase`              | `/tie on`
| `Manual`             | `/env auto off`
| `BurnNow`            | `/burn force on`<br/>Will burn until force burn is toggled off again.
| `RaidAssistNum <N>`  | `/cc ass smart <N>`
