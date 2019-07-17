# -*- coding: utf-8 -*-

module Std

  ##
  # Class: Std::ClimbingBox
  #
  # This class adds a climbing plant into a given box.
  # Use it like this:
  #
  # <pre class="source">
  # c = ClimbingBox.new(UIDS[14], count: 4)
  # c.attach
  # </pre>
  #
  # This will create a climbing plant with 4 middle sprites (and a top
  # sprite) and attaches it to the box with UID 14. When that one gets
  # activated, the climbing plant will come out of it.
  #
  # Once you called #attach, Climbing boxes automatically save its
  # state to a savegame, so you don’t have to do that manually by
  # hooking into the level save and load events.
  class ClimbingBox

    # All the plant sprites.
    attr_reader :sprites
    # The underlying Box instance.
    attr_reader :box

    ##
    # Method: Std::ClimbingBox::new
    #
    #   new( box, opts ) → a_climbing_box
    #
    # Create a new climbing plant box.
    #
    # <h4>Parameters</h4>
    #
    # <dl>
    # <dt>box</dt>
    # <dd>A Box instance to attach the climbing plant to.</dd>
    # <dt>opts</dt>
    # <dd>Hash for keyword arguments.
    #   <dl>
    #   <dt>count</dt>
    #   <dd>The number of sprites the plant shall have.</dd>
    #   <dt>middle_graphic = "ground/green_1/ladder.png"</dt>
    #   <dd>Middle graphic of the plant.</dd>
    #   <dt>top_graphic = "ground/green_1/ladder_head.png"</dt>
    #   <dd>Top graphci of the plant.</dd>
    #   </dl>
    # </dl>
    def initialize(box, opts)
      opts[:count]          || raise(ArgumentError, "Required keyword argument :count missing!")
      opts[:middle_graphic] ||= "ground/green_1/ladder.png"
      opts[:top_graphic]    ||= "ground/green_1/ladder_head.png"

      @activated = false
      @box = box.kind_of?(Integer) ? UIDS[box] : box
      @sprites = []

      # Middle sprites
      1.upto(opts[:count]) do |i|
        s = Sprite.new(opts[:middle_graphic])
        box_x, box_y, box_w, box_h = @box.rect
        plant_x, plant_y, plant_w, plant_h = s.rect
        start_x = box_x + (box_w / ( 2 * box_x * plant_w)) # center plant tile on the box
        start_y = box_y - plant_y - (plant_h * i)
        s.massive_type = :climbable
        s.start_at(start_x, start_y)
        s.suppress_save = true
        s.show

        @sprites << s
      end

      # Top sprite
      top_sprite = Sprite.new(opts[:top_graphic])
      top_sprite.start_at(@sprites.last.x, @sprites.last.y - top_sprite.rect[3])
      top_sprite.massive_type = :climbable
      top_sprite.show
      top_sprite.suppress_save = true
      @sprites << top_sprite

      # First hide all the sprites.
      @sprites.each{|sprite| sprite.disable}
    end

    ##
    # Method: Std::ClimbingBox#attach
    #
    #   attach()
    #
    # Attach the climbing plant to its box.
    def attach
      spritelist = @sprites.dup # We will empty this array on showing the plant
      @timer = nil
      @box.on_activate do
        Audio.play_sound("stomp_4.ogg")

        @timer = Timer.every(250) do
          spritelist.shift.enable

          @timer.stop if spritelist.empty?
        end
        @activated = true
      end

      # Automatic saving and loading.
      Level.on_save do |store|
        store["_ssl"] ||= {}
        store["_ssl"]["climbingboxes"] ||= {}
        store["_ssl"]["climbingboxes"][@box.uid] = @activated
      end

      Level.on_load do |store|
        if store["_ssl"] && store["_ssl"]["climbingboxes"] && store["_ssl"]["climbingboxes"][@box.uid.to_s]
          @sprites.each{|sprite| sprite.enable}
        end
      end
    end

    ##
    # Method: Std::ClimbingBox#activated?
    #
    #   activated?() → bool
    #
    # TODO: Docs
    def activated?
      @activated
    end

    ##
    # Method: Std::ClimbingBox#show_plant
    #
    #   show_plant()
    #
    # Immediately show the entire climbing plant.
    def show_plant
      @sprites.each{|sprite| sprite.enable}
      @activated = true
    end

    ##
    # Method: Std::ClimbingBox#hide_plant
    #
    #   hide_plant()
    #
    # Immediately hide the entire climbing plant.
    def hide_plant
      @sprites.each{|sprite| sprite.disable}
      @activated = false
    end

  end

end
