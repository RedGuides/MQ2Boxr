## MQ2Boxr

This plugin provides a command to control boxes running CWTN plugins,
KissAssist, MuleAssist, rgmercs, Entropy, and XGen. The idea is to have one common command,
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
| `/boxr BurnOff`                 | Turns Burn Function Off
| `/boxr BurnNAmed`               | Turns on Burn Functions for Named
| `/boxr RaidAssistNum <1\|2\|3>` | Toggles which Raid MA the character will assist. This command is not supported on the Emulator build
| `/boxr Debug <on\|off>`         | Turn on/off MQ2Boxr debug logging
| `/boxr Help`                    | Print help text

### Specifics

The following sections document how each boxr command maps to the different
plugins/macros.

Boxr has a **key** for each automation, which is listed after the name in the following
sections. This key is currently only used to identify the current automation in the
TLO (see below).

#### CWTN plugins (`cwtn`)

The mapping to CWTN commands is very straight-forward

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/war mode assist`
| `Chase`              | `/war mode chase`
| `Manual`             | `/war mode manual`
| `BurnNow`            | `/war BurnNow`
| `BurnOff`            | `/war BurnAlways off` <br/> `/war BurnAllNamed off`
| `BurnNamed`          | `/war BurnAllNamed on`
| `RaidAssistNum <N>`  | `/war RaidAssistNum <N>`

#### KissAssist (`kissassist`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `BurnNow`            | `/burn on doburn`
| `BurnOff`            | `/burn off`
| `BurnNamed`          | `/burn on`
| `RaidAssistNum <N>`  | `/switchma <Name of Raid MA #N> tank 1`

#### MuleAssist (`muleassist`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `BurnNow`            | `/burn`
| `RaidAssistNum <N>`  | `/changema <Name of Raid MA #N>`

#### AlsoKissAssist (`alsokissassist`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/camphere on`
| `Chase`              | `/chaseon`
| `Manual`             | `/chaseoff` <br/> `/camphere off`
| `BurnNow`            | `/burn on doburn`
| `RaidAssistNum <N>`  | `/switchma <Name of Raid MA #N>`

#### rgmercs (`rgmercs`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/rg camphard`
| `Chase`              | `/rg chaseon`
| `Manual`             | `/rg chaseoff`<br/>`/rg campoff`
| `BurnNow`            | *Not supported*
| `RaidAssistNum <N>`  | `/rg AssistOutside 1`<br />`/rg OutsideAssistList <Name of Raid MA #N>`

#### rgmercs lua (`rgmercs lua`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/rgl camphard`
| `Chase`              | `/rgl chaseon`
| `Manual`             | `/rgl chaseoff`<br/>`/rgl campoff`
| `BurnNow`            | `/rgl set BurnAlways 1`
| `BurnOff`            | `/rgl set BurnAlways 0` <br/>`/rgl set BurnAuto 0`<br/>`/rgl set BurnNamed 0`
| `BurnNamed`          | `/rgl set BurnNamed 1`
| `RaidAssistNum <N>`  | `/rgl set RaidAssistTarget <N>`

#### Entropy (`entropy`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/tie off` <br/>`/home set on`
| `Chase`              | `/tie on`
| `Manual`             | `/env auto off`
| `BurnNow`            | `/burn force on`<br/>Will burn until force burn is toggled off again.
| `BurnOff`            | `/burn force off` <br/>`/burn auto off`
| `BurnNamed`          | `/burn auto on`
| `RaidAssistNum <N>`  | `/cc ass smart <N>`

#### XGen (`xgen`)

| Boxr command         | Action
|----------------------|-------------
| `Camp`               | `/cc camp on` <br/>`/cc follow off`
| `Chase`              | `/cc follow` <br/>`/cc camp off`
| `Manual`             | `/cc manual`
| `BurnNow`            | `/cc burnonce`
| `BurnOff`            | `/cc burn off`
| `BurnNamed`          | `/cc burn on`
| `RaidAssistNum <N>`  | `/cc setassist <N>`

### TLO

MQ2Boxr provides a TLO, `Boxr`, which exposes the following data:

| Member               | Type    |Data                                                 | Values
|----------------------|---------|-----------------------------------------------------|--------------
| `${Boxr.Current}`    | String  | Key of the automation that is controlling the toon  | `cwtn`, `enthropy`, `kissassist`, etc
| `${Boxr.Paused}`     | Boolean | Indicates whether the automation is currenly paused | `TRUE`, `FALSE`
