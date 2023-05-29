<script lang="ts">
export default {
    props: ['radius', 'xcenter', 'ycenter']
}
</script>

<template>
    <div class="circle"></div>
</template>

<style scoped>
.circle {
    position: absolute;
    border-radius: v-bind(radius);
    top: v-bind(ycenter);
    left: v-bind(xcenter);
    height: 100px;
    width: 100px;
    border: 2px solid black;
}
</style>