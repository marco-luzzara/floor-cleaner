<script lang="ts">
export default {
    props: ['width', 'height', 'xcenter', 'ycenter']
}
</script>

<template>
    <div class="circle"></div>
</template>

<style scoped>
.circle {
    position: absolute;
    border-radius: 100%;
    top: v-bind(ycenter);
    left: v-bind(xcenter);
    height: v-bind(height);
    width: v-bind(width);
    border: 2px solid black;
}
</style>