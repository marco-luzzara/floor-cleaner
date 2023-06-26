<script setup lang="ts">
import { computed } from 'vue'

const props = defineProps({
    image: {
        type: String,
    },
    class: {
        type: String,
    },
})

const imageStyle = {
    color: 'white',
    backgroundImage: `url(${props.image})`,
    backgroundRepeat: 'no-repeat',
    backgroundPosition: 'center',
    backgroundSize: 'contain'
}

const style = computed(() => imageStyle)
style.value.backgroundSize = 'contain';
</script>

<template>
    <div class="grid grid-cols-2 w-full h-full auto-rows-fr">
        <div class="slidev-layout default" :class="props.class">
            <slot />
        </div>
        <div class="w-full w-full" :style="style" />
    </div>
</template>