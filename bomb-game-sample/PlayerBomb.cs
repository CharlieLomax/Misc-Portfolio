using Godot;
using System;
using System.Threading.Tasks;

public class PlayerBomb : Area2D
{
    /** Current force for the explosion */
    public float explosionForce { get; private set; } = 500;
    
    [Export]
    /** Maximum explosion force */
    private float maxExplosionForce = 1000;

    [Export]
    /** Increase in explosion force per second the mouse is held */
    private float explosionForceIncrement = 500;

    [Export]
    /** Path to this object's animated sprite node */
    private NodePath animSpritePath;

    [Export]
    /** Path to this object's animation player node */
    private NodePath animPlayerPath;

    /** This object's animated sprite node */
    private AnimatedSprite _animSprite;

    /** This object's animation player node */
    private AnimationPlayer _animPlayer;

    [Export]
    /** Scene to instance when an explosion is triggered */
    private PackedScene explosion;

    /** Point the bomb's position is locked to */
    private Position2D _positionLock = null;

    /** Bomb's velocity */
    private Vector2 _velocity = Vector2.Zero;

    /** Whether this bomb affects the player */
    private bool _affectPlayer = false;

    private Node _defaultParent;

    private CollisionShape2D _collider;

    private bool _charging = true;

    public bool IsCharged => explosionForce > maxExplosionForce;

    public override void _Ready()
    {
        _animSprite = GetNode<AnimatedSprite>(animSpritePath);
        _animPlayer = GetNode<AnimationPlayer>(animPlayerPath);
        _defaultParent = GetParent().GetParent();
        _collider = GetNode<CollisionShape2D>("Collider");
        _collider.Disabled = true;
    }

    public override void _PhysicsProcess(float delta)
    {
        //base._PhysicsProcess(delta);

        if (_velocity != Vector2.Zero)
        {
            Position += delta * _velocity;
        }
    }

    public override void _Process(float delta)
    {
        if(_positionLock != null)
        {
            GlobalPosition = _positionLock.GlobalPosition;
        }

        // Increment the explosion force as long as it is held by the ship
        if (_charging && !IsCharged)
        {
            explosionForce += explosionForceIncrement * delta;
        }
    }

    /// <summary>
    /// Detonates the bomb, destroying the object and creating an explosion in its place
    /// </summary>
    public void Detonate()
    {
        Explosion ex = explosion.Instance<Explosion>();
        ex.GlobalPosition = GlobalPosition;
        ex.Modulate = _animSprite.Modulate;
        ex.Power = explosionForce;
        ex.AffectPlayer = _affectPlayer;
        GetTree().Root.CallDeferred("add_child", ex);
        QueueFree();
    }
    
    /// <summary>
    /// Binds the position of this bomb to the given position node
    /// </summary>
    /// <param name="pos">Position node to lock to</param>
    public void LockPosition(Position2D pos)
    {
        _positionLock = pos;
    }

    public void StopCharging()
    {
        _animPlayer.Stop();
        _charging = false;
    }

    /// <summary>
    /// Unlocks the bomb's position and sends it moving with the given velocity
    /// </summary>
    /// <param name="velocity">The velocity to move the bomb with</param>
    public void Fire(Vector2 velocity)
    {
        _positionLock = null;
        StopCharging();
        _velocity = velocity;
        _affectPlayer = true;

        EnableCollider();
    }

    private async void EnableCollider()
    {
        await Task.Delay(100);
        _collider.SetDeferred("disabled", false);
    }

    public void OnPlayerBombBodyEntered(PhysicsBody2D body)
    {
        Detonate();
    }
}
