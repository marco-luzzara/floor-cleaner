<script lang="ts">
export default {
    props: ['from', 'to']
}
</script>

<template>
    <div class="cell">
        test
    </div>
</template>

<style scoped>
.cell {
    position: absolute;
    width: 100%;
    height: 100%;
    /* height: v-bind(height);
    width: v-bind(width); */
    border: 2px solid grey;
}
</style>