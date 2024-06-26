#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <optional>

struct Vertex;

const std::vector<const char*> requested_layers = {
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> required_device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;

    bool is_complete() {
        return graphics_family.has_value() && present_family.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

class Application {
public:
    Application();

    void run();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application();

private:
    // Initializing glfw
    void init_glfw();
    GLFWwindow* window;

    // Callbacks
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void framebuffer_resize_callback(GLFWwindow* window, int width, int height);

    // Initializing vulkan
    void init_vulkan();

    // Instance creation
    void create_instance();
    void check_validation_layer_support();
    VkInstance instance;                                                                                                                                                                                                                                                                                                                                                                                                                                          

    // Debug messenger
    void setup_debug_messenger();
    VkDebugUtilsMessengerEXT debug_messenger;

    // Surface
    void create_surface();
    VkSurfaceKHR surface;

    // Physical device
    void select_physical_device();
    bool is_suitable_device(VkPhysicalDevice);
    bool check_device_extension_support(VkPhysicalDevice);
    void sort_physical_devices(std::vector<VkPhysicalDevice>&);
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    
    // Queue families
    QueueFamilyIndices find_queue_families(VkPhysicalDevice);

    // Logical device
    void create_logical_device();
    VkDevice device;

    // Queues
    VkQueue graphics_queue;
    VkQueue present_queue;

    // Swapchain queries
    SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice);
    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR>&);
    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR>&);
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR&);

    // Swapchain
    void create_swap_chain();
    void recreate_swap_chain();
    void cleanup_swap_chain();
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;
    std::vector<VkImage> swap_chain_images;
    VkSwapchainKHR swap_chain;
    bool framebuffer_resized = false;

    // Image views
    void create_image_views();
    std::vector<VkImageView> swap_chain_image_views;

    // Render pass
    void create_render_pass();
    VkRenderPass render_pass;

    // Descriptor set layout
    void create_descriptor_set_layout();
    VkDescriptorSetLayout descriptor_set_layout;

    // Graphics pipeline
    void create_graphics_pipeline();
    VkShaderModule create_shader_module(const std::vector<char>&);
    VkPipelineLayout pipeline_layout;
    VkPipeline graphics_pipeline;

    // Framebuffers
    void create_framebuffers();
    std::vector<VkFramebuffer> swap_chain_framebuffers;

    // Model data
    void load_model();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // Buffers
    uint32_t find_memory_type(uint32_t type_filter, VkMemoryPropertyFlags);
    void create_buffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer&, VkDeviceMemory&);
    void copy_buffer(VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceSize);
    void create_vertex_buffer();
    void create_index_buffer();
    void create_uniform_buffers();
    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;
    VkBuffer index_buffer;
    VkDeviceMemory index_buffer_memory;
    std::vector<VkBuffer> uniform_buffers;
    std::vector<VkDeviceMemory> uniform_buffers_memory;
    std::vector<void*> uniform_buffers_pointers;

    // Texture image
    void create_image(uint32_t width, uint32_t height, uint32_t _mip_levels, VkSampleCountFlagBits nr_samples,
                     VkFormat, VkImageTiling, VkImageUsageFlags, 
                     VkMemoryPropertyFlags, VkImage&, VkDeviceMemory&);
    VkImageView create_image_view(VkImage, VkFormat, VkImageAspectFlags, uint32_t _mip_levels);
    void transition_image_layout(VkImage, VkFormat, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t _mip_levels);
    void generate_mipmaps(VkImage, VkFormat, int32_t texture_width, int32_t texture_height, uint32_t _mip_levels);
    void create_texture_image();
    void create_texture_image_view();
    void create_texture_sampler();
    void copy_buffer_to_image(VkBuffer, VkImage, uint32_t width, uint32_t height);
    VkImage texture_image;
    uint32_t mip_levels;
    VkDeviceMemory texture_image_memory;
    VkImageView texture_image_view;
    VkSampler texture_sampler;

    // Multisampling
    VkSampleCountFlagBits get_max_usable_sample_count();
    void create_color_resource();
    VkSampleCountFlagBits msaa_samples = VK_SAMPLE_COUNT_1_BIT;
    VkImage color_image;
    VkDeviceMemory color_image_memory;
    VkImageView color_image_view;

    // Depth buffer
    void create_depth_resource();
    VkFormat find_supported_format(const std::vector<VkFormat>&, VkImageTiling, VkFormatFeatureFlags);
    VkFormat find_depth_format();
    VkImage depth_image;
    VkDeviceMemory depth_image_memory;
    VkImageView depth_image_view;

    // Decriptor pool
    void create_descriptor_pool();
    VkDescriptorPool descriptor_pool;

    // Descriptor sets
    void create_descriptor_sets();
    std::vector<VkDescriptorSet> descriptor_sets;

    // Command pool
    void create_command_pool();
    VkCommandPool command_pool;

    // Command buffer
    void create_command_buffers();
    void record_command_buffer(VkCommandBuffer, uint32_t image_index);
    VkCommandBuffer begin_single_time_commands();
    void end_single_time_commands(VkCommandBuffer);
    std::vector<VkCommandBuffer> command_buffers;

    // Synchronization objects
    void create_sync_objects();
    std::vector<VkSemaphore> image_available_semaphores;
    std::vector<VkSemaphore> render_finished_semaphores;
    std::vector<VkFence> in_flight_fences;

    // Drawing
    void draw_frame();
    void update_uniform_buffer(uint32_t);
    uint32_t current_frame = 0;
    float time;
};

#endif