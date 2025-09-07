---
tags:
  - command
---

# /boxr

## Syntax

<!--cmd-syntax-start-->
```eqcommand
/boxr [option] | [option <setting>]
```
<!--cmd-syntax-end-->

## Description

<!--cmd-desc-start-->
Sends commands that control a variety of different software.
<!--cmd-desc-end-->

## Options

| Option | Description |
|--------|-------------|
| `Pause` | Pause the character |
| `Unpause` | Unpause the character |
| `Camp` | Camp in one spot, assist the MA within camp radius, and return to camp after battle |
| `Chase` | Chase the MA, and assist in battle |
| `Manual` | Do not chase, do not return to camp. This state behaves a little bit different between different boxes, see Specifics below |
| `BurnNow` | Burn current target |
| `RaidAssistNum <1|2|3>` | Toggles which Raid MA the character will assist. Currently not available in emulator clients |
| `Debug <on|off>` | Turn on/off debug logging for MQ2Boxr. This command is not relayed to CWTN/Kiss/rgmercs |
| `Help` | Print help text |

## Examples

- Using [MQ2EQBC](../mq2eqbc/index.md), this will issue a chase command to the rest of the group:
; /bcg //boxr chase
- Using [MQ2DanNet](../mq2dannet/index.md) this will pause all toons in zone:
; /dgza /boxr pause