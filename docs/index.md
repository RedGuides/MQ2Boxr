---
tags:
  - plugin
resource_link: "https://www.redguides.com/community/resources/mq2boxr.2036/"
support_link: "https://www.redguides.com/community/threads/mq2boxr.76812/"
repository: "https://github.com/RedGuides/MQ2Boxr"
authors: "Sidien, dannuic"
tagline: "A unified way to control boxes running a variety of software including CWTN, KissAssist, MuleAssist, RGMercs, and Entropy"
---

# MQ2Boxr

<!--desc-start-->
This plugin provides a command to control boxes running CWTN plugins, KissAssist, MuleAssist, RGMercs, RGMercs Lua, Entropy, XGen, and AlsoKissAssist. The idea is to have one common command, which can be broadcast to all boxed characters, regardless of what they are
running.
<!--desc-end-->

## Commands

<a href="cmd-boxr/">
{% 
  include-markdown "projects/mq2boxr/cmd-boxr.md" 
  start="<!--cmd-syntax-start-->" 
  end="<!--cmd-syntax-end-->" 
%}
</a>
:    {% include-markdown "projects/mq2boxr/cmd-boxr.md" 
        start="<!--cmd-desc-start-->" 
        end="<!--cmd-desc-end-->" 
        trailing-newlines=false 
     %} {{ readMore('projects/mq2boxr/cmd-boxr.md') }}

## Settings

The following tables document how each boxr command maps to the different plugins/macros.

Boxr has a **key** for each automation, which is listed after the name in the following
sections. This key is currently only used to identify the current automation in the
TLO (see below).

### KissAssist (`kissassist`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/camphere on` |
| Chase | `/chaseon` |
| Manual | `/chaseoff`<br/>`/camphere off` |
| BurnNow | `/burn` |
| RaidAssistNum `<N>` | `/switchma <Name of Raid MA #N>` |

### CWTN Plugins (`cwtn`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/war mode assist` |
| Chase | `/war mode chase` |
| Manual | `/war mode manual` |
| BurnNow | `/war BurnNow` |
| RaidAssistNum `<N>` | `/war RaidAssistNum` |

### RGMercs (`rgmercs`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/rg camphard` |
| Chase | `/rg chaseon` |
| Manual | `/rg chaseoff` <br/> `/rg campoff` |
| BurnNow | *not supported* |
| RaidAssistNum `<N>` | `/rg AssistOutside 1` <br/> `/rg OutsideAssistList <Name of Raid MA #N>` |

### RGMercs Lua (`rgmercs lua`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/rgl camphard` |
| Chase | `/rgl chaseon` |
| Manual | `/rgl chaseoff` <br/> `/rgl campoff` |
| BurnNow | `/rgl set BurnAlways 1` |
| RaidAssistNum `<N>` | *not supported* |

### MuleAssist (`muleassist`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/camphere on` |
| Chase | `/chaseon` |
| Manual | `/chaseoff`<br/>`/camphere off` |
| BurnNow | `/burn` |
| RaidAssistNum `<N>` | `/changema <Name of Raid MA #N>` |

### Entropy (`entropy`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/tie off`<br/>`/home set on` |
| Chase | `/tie off` |
| Manual | `/env auto off` |
| BurnNow | `/burn force on`<br/>[Will burn until force burn is toggled off again.](https://entropy.exspes.com/wiki/index.php/Burn#force) |
| RaidAssistNum `<N>` | `/cc ass smart <N>` |

### XGen (`xgen`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/cc camp on`<br/>`/cc follow off` |
| Chase | `/cc follow`<br/>`/cc camp off` |
| Manual | `/cc manual` |
| BurnNow | `/cc burnonce` |
| RaidAssistNum `<N>` | `/cc setassist <N>` |

### AlsoKissAssist (`alsokissassist`)

| Boxr command | Action |
|--------------|--------|
| Camp | `/camphere on` |
| Chase | `/chaseon` |
| Manual | `/chaseoff`<br/>`/camphere off` |
| BurnNow | `/burn on doburn` |
| RaidAssistNum `<N>` | `/switchma <Name of Raid MA #N>` |

## Top-Level Objects

## [Boxr](tlo-boxr.md)
{% include-markdown "projects/mq2boxr/tlo-boxr.md" start="<!--tlo-desc-start-->" end="<!--tlo-desc-end-->" trailing-newlines=false %} {{ readMore('projects/mq2boxr/tlo-boxr.md') }}

## DataTypes

## [Boxr](datatype-boxr.md)
{% include-markdown "projects/mq2boxr/datatype-boxr.md" start="<!--dt-desc-start-->" end="<!--dt-desc-end-->" trailing-newlines=false %} {{ readMore('projects/mq2boxr/datatype-boxr.md') }}

## Members
{% include-markdown "projects/mq2boxr/datatype-boxr.md" start="<!--dt-members-start-->" end="<!--dt-members-end-->" %}
{% include-markdown "projects/mq2boxr/datatype-boxr.md" start="<!--dt-linkrefs-start-->" end="<!--dt-linkrefs-end-->" %}
