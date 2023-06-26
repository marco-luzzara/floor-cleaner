<script setup lang="ts">
type LineDirections = "up" | "right" | "down" | "left"

export interface Props {
    from?: LineDirections,
    to?: LineDirections,
    textColor?: string,
    color?: string,
    invisible?: boolean
    isDashed?: boolean
    centerContent?: string
}
const props = withDefaults(defineProps<Props>(), {
    invisible: false,
    centerContent: '',
    isDashed: false
})
</script>

<template>
    <div class="cell" v-bind:class="{ 'full-cell': !invisible && !isDashed, 'dashed-cell': !invisible && isDashed }">
        <hr class="line up-line" v-if="from === 'up' || to === 'up'" />
        <hr class="line right-line" v-if="from === 'right' || to === 'right'" />
        <hr class="line down-line" v-if="from === 'down' || to === 'down'" />
        <hr class="line left-line" v-if="from === 'left' || to === 'left'" />
        <div class="centerContent">
            <b class="centerContentText" v-if="centerContent !== ''">{{ centerContent }}</b>
        </div>
    </div>
</template>

<style scoped>
.centerContent {
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);
    position: absolute;
}

.centerContentText {
    font-size: 1.5em;
    color: v-bind('props.textColor');
}

.cell {
    position: relative;
    background-color: v-bind('props.color');
}

.full-cell {
    border: 2px solid grey;
}

.dashed-cell {
    border: 2px dashed grey;
}

.line {
    position: absolute;
    border: 0.2em dashed red;
}

.up-line {
    height: 50%;
    left: 50%;
}

.right-line {
    width: 50%;
    top: 50%;
    left: 50%;
}

.down-line {
    height: 50%;
    top: 50%;
    left: 50%;
}

.left-line {
    width: 50%;
    top: 50%;
}
</style>