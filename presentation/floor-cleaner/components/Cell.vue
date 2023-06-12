<script setup lang="ts">
type LineDirections = "up" | "right" | "down" | "left"

export interface Props {
    from?: LineDirections,
    to?: LineDirections,
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
    <div v-bind:class="{ cell: !invisible && !isDashed, 'dashed-cell': !invisible && isDashed }">
        <hr class="line up-line" v-if="from === 'up' || to === 'up'" />
        <hr class="line right-line" v-if="from === 'right' || to === 'right'" />
        <hr class="line down-line" v-if="from === 'down' || to === 'down'" />
        <hr class="line left-line" v-if="from === 'left' || to === 'left'" />
        <div class="centerContent">
            <b v-if="centerContent !== ''">{{ centerContent }}</b>
        </div>
    </div>
</template>

<style scoped>
.centerContent {
    width: 100%;
    height: 100%;
    text-align: center;
    position: relative;
}

.cell {
    position: relative;
    border: 2px solid grey;
}

.dashed-cell {
    position: relative;
    border: 2px dashed grey;
}

.line {
    position: absolute;
    border: 0.2em dashed red;
}

.up-line {
    height: 50%;
    left: 50%;
    right: 50%;
}

.right-line {
    width: 50%;
    top: 50%;
    bottom: 50%;
    left: 50%;
}

.down-line {
    height: 50%;
    top: 50%;
    left: 50%;
    right: 50%;
}

.left-line {
    width: 50%;
    top: 50%;
    bottom: 50%;
}
</style>